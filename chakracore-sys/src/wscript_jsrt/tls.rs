use crate::jsrt::{
    ChakraRt, JsContextRef, JsError, JsParseScriptAttributes, JsRuntimeAttributes, JsRuntimeHandle,
    JsSourceContext, JsValueRef,
};
use crate::rt_interface::ChakraRTInterface;
use crate::wscript_jsrt::ffi::{RuntimeThreadData, SetEvent, WaitForMultipleObjects};
use crate::wscript_jsrt::{Handle, WScript};
use cxx::SharedPtr;
use std::cell::Cell;
use std::rc::{Rc, Weak};
use std::sync::{Mutex, mpsc};
use tokio::sync::oneshot;

pub(super) struct ThreadData {
    pub(super) inner: SharedPtr<RuntimeThreadData>,
    initial_source: String,
    pub(super) report_queue_tx: mpsc::Sender<String>,
    report_queue_rx: mpsc::Receiver<String>,
    parent_report_tx: Option<mpsc::Sender<String>>,
    leaving: Cell<bool>,
    thread_handle: Handle,
    parent: Weak<ThreadData>,
    receive_broadcast_callback_func: Cell<Option<JsValueRef>>,
    children: Mutex<Vec<Rc<ThreadData>>>,
}

impl ThreadData {
    pub(super) fn new() -> Self {
        let inner = RuntimeThreadData::New();
        let (tx, rx) = mpsc::channel();
        Self {
            inner,
            initial_source: String::new(),
            report_queue_tx: tx,
            report_queue_rx: rx,
            parent_report_tx: None,
            leaving: Cell::new(false),
            thread_handle: Handle::default(),
            parent: Weak::new(),
            receive_broadcast_callback_func: Cell::default(),
            children: Mutex::default(),
        }
    }

    pub(super) fn new_with_initial_source(
        source: &str,
        parent_report_tx: mpsc::Sender<String>,
        thread_handle: Handle,
        parent: Weak<ThreadData>,
    ) -> Self {
        let initial_source = source.to_owned();
        let inner = RuntimeThreadData::NewWithHandle(thread_handle.clone());
        let (tx, rx) = mpsc::channel();
        Self {
            inner,
            initial_source,
            report_queue_tx: tx,
            report_queue_rx: rx,
            parent_report_tx: Some(parent_report_tx),
            leaving: Cell::new(false),
            thread_handle,
            parent,
            receive_broadcast_callback_func: Cell::default(),
            children: Mutex::default(),
        }
    }

    #[tracing::instrument(skip(self, wscript), err)]
    pub(super) fn thread_proc(
        &self,
        wscript: &WScript,
        tx_initial_script_completed: oneshot::Sender<()>,
    ) -> Result<u32, ThreadProcError> {
        let mut runtime = JsRuntimeHandle::default();
        unsafe {
            ChakraRTInterface::JsCreateRuntime(
                JsRuntimeAttributes::JsRuntimeAttributeNone,
                &raw mut runtime,
            )
            .as_result()?;
        }

        let mut context = JsContextRef::default();
        unsafe {
            ChakraRTInterface::JsCreateContext(runtime, &raw mut context).as_result()?;
            ChakraRTInterface::JsSetCurrentContext(context).as_result()?;
        }
        wscript.initialize()?;

        let mut script_source = JsValueRef::default();
        unsafe {
            ChakraRTInterface::JsCreateExternalArrayBuffer(
                &self.initial_source,
                &raw mut script_source,
            )
            .as_result()?;
        }
        let full_path = ChakraRt::create_string("agent source")?;

        unsafe {
            ChakraRTInterface::JsRun(
                script_source,
                JsSourceContext(WScript::get_next_source_context()),
                *full_path,
                JsParseScriptAttributes::JsParseScriptAttributeNone,
                std::ptr::null_mut(),
            )
            .as_result()?;
        }

        tx_initial_script_completed
            .send(())
            .map_err(|_| ThreadProcError::NotificationFailed)?;

        let handles = [
            self.inner.get_received_broadcast_event(),
            self.inner.get_shutdown_event(),
        ];

        // loop waiting for work
        loop {
            let wait = WaitForMultipleObjects(&handles, false);

            if wait == 0 {
                self.on_receive_broadcast()?;
            }

            if wait == 1 || self.leaving.get() {
                self.on_shutdown(runtime);
                return Ok(0);
            }

            if wait != 0 {
                unreachable!();
            }
        }
    }

    fn on_receive_broadcast(&self) -> Result<(), JsError> {
        let global = ChakraRt::get_global_object()?;
        let mut shared_buffer = JsValueRef::default();
        unsafe {
            let parent = self.parent.upgrade().unwrap();
            let shared_content = parent.inner.get_shared_content();
            ChakraRTInterface::JsCreateSharedArrayBufferWithSharedContent(
                shared_content,
                &raw mut shared_buffer,
            );
        }
        let shared_content_length = ChakraRt::double_to_number(1.)?;
        let mut args = [*global, shared_buffer, shared_content_length];
        if let Some(callback_func) = self.receive_broadcast_callback_func.get() {
            unsafe {
                ChakraRTInterface::JsCallFunction(
                    callback_func,
                    args.as_mut_ptr(),
                    3,
                    std::ptr::null_mut(),
                );
            }
        }

        Ok(())
    }

    fn on_shutdown(&self, runtime: JsRuntimeHandle) {
        if let Some(callback_func) = self.receive_broadcast_callback_func.get() {
            unsafe {
                ChakraRTInterface::JsRelease(callback_func.as_js_ref(), std::ptr::null_mut());
            }
        }
        ChakraRTInterface::JsSetCurrentContext(JsContextRef::default());
        ChakraRTInterface::JsDisposeRuntime(runtime);
    }

    pub(super) fn enqueue_report_to_parent(&self, report: String) -> Result<(), ReportQueueError> {
        if let Some(tx) = &self.parent_report_tx {
            tx.send(report)?;
        }

        Ok(())
    }

    pub(super) fn try_dequeue_report(&self) -> Result<Option<String>, ReportQueueError> {
        match self.report_queue_rx.try_recv() {
            Ok(x) => Ok(Some(x)),
            Err(mpsc::TryRecvError::Empty) => Ok(None),
            Err(x) => Err(ReportQueueError::ReportRecvError(x)),
        }
    }

    pub(super) fn set_leaving(&self, leaving: bool) {
        self.leaving.set(leaving);
    }

    pub(super) fn set_receive_broadcast_callback_func(&self, func: JsValueRef) {
        self.receive_broadcast_callback_func.set(Some(func));
    }

    pub(super) fn get_receive_broadcast_callback_func(&self) -> Option<JsValueRef> {
        self.receive_broadcast_callback_func.get()
    }

    pub(super) fn add_child(&self, child: Rc<ThreadData>) {
        let mut children = self.children.lock().unwrap();
        children.push(child);
    }

    pub(super) fn broadcast_to_children(&self) {
        let children = self.children.lock().unwrap();
        for child in children.iter() {
            child.inner.broadcast();
        }
    }
}

impl Drop for ThreadData {
    fn drop(&mut self) {
        let children = self.children.lock().unwrap();
        if !children.is_empty() {
            let mut child_handles = Vec::new();
            for child in children.iter() {
                child_handles.push(child.thread_handle.clone());
                SetEvent(child.inner.get_shutdown_event());
            }

            let ret = WaitForMultipleObjects(&child_handles, true);
            debug_assert_eq!(ret, 0);
        }
    }
}

#[derive(thiserror::Error, Debug)]
pub(super) enum ReportQueueError {
    #[error("Failed to send report")]
    ReportSendError(#[from] mpsc::SendError<String>),
    #[error("Failed to receive report")]
    ReportRecvError(#[from] mpsc::TryRecvError),
}

#[derive(thiserror::Error, Debug)]
pub(super) enum ThreadProcError {
    #[error(transparent)]
    JsError(#[from] JsError),
    #[error("Failed to notify parent of initial script completion")]
    NotificationFailed,
}
