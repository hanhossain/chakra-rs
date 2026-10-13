use crate::jsrt::{
    ChakraRt, JsContextRef, JsError, JsParseScriptAttributes, JsRuntimeAttributes, JsRuntimeHandle,
    JsSourceContext, JsValueRef,
};
use crate::rt_interface::ChakraRTInterface;
use crate::wscript_jsrt::WScript;
use crate::wscript_jsrt::ffi::RuntimeThreadData;
use cxx::SharedPtr;
use std::sync::mpsc;
use tokio::sync::oneshot;

pub(super) struct ThreadData {
    pub(super) inner: SharedPtr<RuntimeThreadData>,
    initial_source: String,
    pub(super) report_queue_tx: mpsc::Sender<String>,
    report_queue_rx: mpsc::Receiver<String>,
    parent_report_tx: Option<mpsc::Sender<String>>,
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
        }
    }

    pub(super) fn new_with_initial_source(
        source: &str,
        parent_report_tx: mpsc::Sender<String>,
    ) -> Self {
        let initial_source = source.to_owned();
        let inner = RuntimeThreadData::New();
        let (tx, rx) = mpsc::channel();
        Self {
            inner,
            initial_source,
            report_queue_tx: tx,
            report_queue_rx: rx,
            parent_report_tx: Some(parent_report_tx),
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

        let res = self.inner.ThreadProc(runtime);
        Ok(res)
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
