use crate::jsrt::{
    ChakraRt, JsContextRef, JsError, JsParseScriptAttributes, JsRuntimeAttributes, JsRuntimeHandle,
    JsSharedArrayBufferContentHandle, JsSourceContext, JsValueRef,
};
use crate::rt_interface::ChakraRTInterface;
use crate::wscript_jsrt::ffi::{CloseHandle, WaitForMultipleObjects};
use crate::wscript_jsrt::{Handle, WScript};
use std::cell::Cell;
use std::rc::Rc;
use std::sync::{Mutex, mpsc};
use tokio::sync::oneshot;

#[derive(Eq, PartialEq)]
pub(super) enum ThreadEvent {
    Broadcast(JsSharedArrayBufferContentHandle),
    Shutdown,
}

pub(super) struct ThreadData {
    initial_source: String,
    pub(super) report_queue_tx: mpsc::Sender<String>,
    report_queue_rx: mpsc::Receiver<String>,
    parent_report_tx: Option<mpsc::Sender<String>>,
    leaving: Cell<bool>,
    thread_handle: Handle,
    receive_broadcast_callback_func: Cell<Option<JsValueRef>>,
    children: Mutex<Vec<(Rc<ThreadData>, mpsc::Sender<ThreadEvent>)>>,
    events: Option<mpsc::Receiver<ThreadEvent>>,
}

impl ThreadData {
    pub(super) fn new() -> Self {
        let (tx, rx) = mpsc::channel();
        Self {
            initial_source: String::new(),
            report_queue_tx: tx,
            report_queue_rx: rx,
            parent_report_tx: None,
            leaving: Cell::new(false),
            thread_handle: Handle::default(),
            receive_broadcast_callback_func: Cell::default(),
            children: Mutex::default(),
            events: None,
        }
    }

    pub(super) fn new_with_initial_source(
        source: &str,
        parent_report_tx: mpsc::Sender<String>,
        thread_handle: Handle,
        events_rx: mpsc::Receiver<ThreadEvent>,
    ) -> Self {
        let initial_source = source.to_owned();
        let (tx, rx) = mpsc::channel();
        Self {
            initial_source,
            report_queue_tx: tx,
            report_queue_rx: rx,
            parent_report_tx: Some(parent_report_tx),
            leaving: Cell::new(false),
            thread_handle,
            receive_broadcast_callback_func: Cell::default(),
            children: Mutex::default(),
            events: Some(events_rx),
        }
    }

    #[tracing::instrument(skip(self, wscript), err)]
    pub(super) fn thread_proc(
        &self,
        wscript: &WScript,
        tx_initial_script_completed: oneshot::Sender<()>,
    ) -> Result<u32, ThreadProcError> {
        let runtime = ChakraRt::create_runtime(JsRuntimeAttributes::JsRuntimeAttributeNone)?;

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

        // loop waiting for work
        let Some(events) = &self.events else {
            return Ok(0);
        };

        for event in events {
            match event {
                ThreadEvent::Broadcast(shared_content) => {
                    self.on_receive_broadcast(shared_content)?;
                    if self.leaving.get() {
                        self.on_shutdown(runtime);
                        return Ok(0);
                    }
                }
                ThreadEvent::Shutdown => {
                    self.on_shutdown(runtime);
                    return Ok(0);
                }
            }
        }

        Ok(0)
    }

    fn on_receive_broadcast(
        &self,
        shared_content: JsSharedArrayBufferContentHandle,
    ) -> Result<(), JsError> {
        let global = ChakraRt::get_global_object()?;
        let mut shared_buffer = JsValueRef::default();
        unsafe {
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

    pub(super) fn add_child(&self, child: Rc<ThreadData>, events_tx: mpsc::Sender<ThreadEvent>) {
        let mut children = self.children.lock().unwrap();
        children.push((child, events_tx));
    }

    pub(super) fn broadcast_to_children(&self, shared_content: JsSharedArrayBufferContentHandle) {
        let children = self.children.lock().unwrap();
        for (_, tx) in children.iter() {
            tx.send(ThreadEvent::Broadcast(shared_content.clone()))
                .unwrap();
        }
    }
}

impl Drop for ThreadData {
    fn drop(&mut self) {
        let children = self.children.lock().unwrap();
        if !children.is_empty() {
            let mut child_handles = Vec::new();
            for (child, tx) in children.iter() {
                child_handles.push(child.thread_handle.clone());
                tx.send(ThreadEvent::Shutdown).unwrap();
            }

            let ret = WaitForMultipleObjects(&child_handles, true);
            debug_assert_eq!(ret, 0);
        }

        if !self.thread_handle.is_null() {
            CloseHandle(&self.thread_handle);
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
