use crate::rt_interface::ffi::JsErrorCode;
use std::ffi::c_void;

#[repr(transparent)]
#[derive(Copy, Clone)]
pub struct JsRuntimeHandle(*mut c_void);

unsafe impl cxx::ExternType for JsRuntimeHandle {
    type Id = cxx::type_id!("JsRuntimeHandle");
    type Kind = cxx::kind::Trivial;
}

impl Default for JsRuntimeHandle {
    fn default() -> Self {
        Self(std::ptr::null_mut())
    }
}

impl JsRuntimeHandle {
    pub fn is_invalid(&self) -> bool {
        self.0 == std::ptr::null_mut()
    }
}

#[repr(transparent)]
pub struct JsContextRef(*mut c_void);

unsafe impl cxx::ExternType for JsContextRef {
    type Id = cxx::type_id!("JsContextRef");
    type Kind = cxx::kind::Trivial;
}

impl Default for JsContextRef {
    fn default() -> Self {
        Self(std::ptr::null_mut())
    }
}

#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("ChakraRtInterface.h");

        type ChakraRTInterface;

        #[Self = "ChakraRTInterface"]
        fn InitializeTestHooks(vargs: &Vec<String>) -> i32;

        type JsErrorCode;
        type JsRuntimeAttributes;
        type JsRuntimeHandle = super::JsRuntimeHandle;
        type JsContextRef = super::JsContextRef;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsCreateRuntime(
            attributes: JsRuntimeAttributes,
            runtime: *mut JsRuntimeHandle,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        fn JsDisposeRuntime(runtime: JsRuntimeHandle) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsCreateContext(
            runtime: JsRuntimeHandle,
            context: *mut JsContextRef,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        fn JsSetCurrentContext(context: JsContextRef) -> JsErrorCode;
    }

    #[derive(Debug)]
    enum JsErrorCode {
        /// Success error code.
        JsNoError = 0,
        /// Category of errors that relates to incorrect usage of the API itself.
        JsErrorCategoryUsage = 0x10000,
        /// An argument to a hosting API was invalid.
        JsErrorInvalidArgument,
        /// An argument to a hosting API was null in a context where null is not allowed.
        JsErrorNullArgument,
        /// The hosting API requires that a context be current, but there is no current context.
        JsErrorNoCurrentContext,
        /// The engine is in an exception state and no APIs can be called until the exception is
        /// cleared.
        JsErrorInExceptionState,
        /// A hosting API is not yet implemented.
        JsErrorNotImplemented,
        /// A hosting API was called on the wrong thread.
        JsErrorWrongThread,
        /// A runtime that is still in use cannot be disposed.
        JsErrorRuntimeInUse,
        /// A bad serialized script was used, or the serialized script was serialized by a different
        /// version of the Chakra engine.
        JsErrorBadSerializedScript,
        /// The runtime is in a disabled state.
        JsErrorInDisabledState,
        /// Runtime does not support reliable script interruption.
        JsErrorCannotDisableExecution,
        /// A heap enumeration is currently underway in the script context.
        JsErrorHeapEnumInProgress,
        /// A hosting API that operates on object values was called with a non-object value.
        JsErrorArgumentNotObject,
        /// A script context is in the middle of a profile callback.
        JsErrorInProfileCallback,
        /// A thread service callback is currently underway.
        JsErrorInThreadServiceCallback,
        /// Scripts cannot be serialized in debug contexts.
        JsErrorCannotSerializeDebugScript,
        /// The context cannot be put into a debug state because it is already in a debug state.
        JsErrorAlreadyDebuggingContext,
        /// The context cannot start profiling because it is already profiling.
        JsErrorAlreadyProfilingContext,
        /// Idle notification given when the host did not enable idle processing.
        JsErrorIdleNotEnabled,
        /// The context did not accept the enqueue callback.
        JsCannotSetProjectionEnqueueCallback,
        /// Failed to start projection.
        JsErrorCannotStartProjection,
        /// The operation is not supported in an object before collect callback.
        JsErrorInObjectBeforeCollectCallback,
        /// Object cannot be unwrapped to IInspectable pointer.
        JsErrorObjectNotInspectable,
        /// A hosting API that operates on symbol property ids but was called with a non-symbol
        /// property id. The error code is returned by JsGetSymbolFromPropertyId if the function is
        /// called with non-symbol property id.
        JsErrorPropertyNotSymbol,
        /// A hosting API that operates on string property ids but was called with a non-string
        /// property id. The error code is returned by existing JsGetPropertyNamefromId if the
        /// function is called with non-string property id.
        JsErrorPropertyNotString,
        /// Module evaluation is called in wrong context.
        JsErrorInvalidContext,
        /// The Module HostInfoKind provided was invalid.
        JsInvalidModuleHostInfoKind,
        /// Module was parsed already when JsParseModuleSource is called.
        JsErrorModuleParsed,
        /// Argument passed to JsCreateWeakReference is a primitive that is not managed by the GC.
        /// No weak reference is required, the value will never be collected.
        JsNoWeakRefRequired,
        /// The `Promise` object is still in the pending state.
        JsErrorPromisePending,
        /// Module was not yet evaluated when JsGetModuleNamespace was called.
        JsErrorModuleNotEvaluated,

        /// Category of errors that relates to errors occurring within the engine itself.
        JsErrorCategoryEngine = 0x20000,
        /// The Chakra engine has run out of memory.
        JsErrorOutOfMemory,
        /// The Chakra engine failed to set the Floating Point Unit state.
        JsErrorBadFPUState,

        /// Category of errors that relates to errors in a script.
        JsErrorCategoryScript = 0x30000,
        /// A JavaScript exception occurred while running a script.
        JsErrorScriptException,
        /// JavaScript failed to compile.
        JsErrorScriptCompile,
        /// A script was terminated due to a request to suspend a runtime.
        JsErrorScriptTerminated,
        /// A script was terminated because it tried to use `eval` or `function` and eval
        /// was disabled.
        JsErrorScriptEvalDisabled,

        /// Category of errors that are fatal and signify failure of the engine.
        JsErrorCategoryFatal = 0x40000,
        /// A fatal error in the engine has occurred.
        JsErrorFatal,
        /// A hosting API was called with object created on different javascript runtime.
        JsErrorWrongRuntime,

        /// Category of errors that are related to failures during diagnostic operations.
        JsErrorCategoryDiagError = 0x50000,
        /// The object for which the debugging API was called was not found
        JsErrorDiagAlreadyInDebugMode,
        /// The debugging API can only be called when VM is in debug mode
        JsErrorDiagNotInDebugMode,
        /// The debugging API can only be called when VM is at a break
        JsErrorDiagNotAtBreak,
        /// Debugging API was called with an invalid handle.
        JsErrorDiagInvalidHandle,
        /// The object for which the debugging API was called was not found
        JsErrorDiagObjectNotFound,
        /// VM was unable to perform the request action
        JsErrorDiagUnableToPerformAction,
        /// Serializer/Deserializer does not support current data
        JsSerializerNotSupported,
        /// Current object is not transferable during serialization
        JsTransferableNotSupported,
        /// Current object is already detached when serialized
        JsTransferableAlreadyDetached,
    }

    enum JsRuntimeAttributes {
        /// No special attributes.
        JsRuntimeAttributeNone = 0x00000000,
        /// The runtime will not do any work (such as garbage collection) on background threads.
        JsRuntimeAttributeDisableBackgroundWork = 0x00000001,
        /// The runtime should support reliable script interruption. This increases the number of
        /// places where the runtime will check for a script interrupt request at the cost of a
        /// small amount of runtime performance.
        JsRuntimeAttributeAllowScriptInterrupt = 0x00000002,
        /// Host will call <c>JsIdle</c>, so enable idle processing. Otherwise, the runtime will
        /// manage memory slightly more aggressively.
        JsRuntimeAttributeEnableIdleProcessing = 0x00000004,
        /// Runtime will not generate native code.
        JsRuntimeAttributeDisableNativeCodeGeneration = 0x00000008,
        /// Using `eval` or function constructor will throw an exception.
        JsRuntimeAttributeDisableEval = 0x00000010,
        /// Runtime will enable all experimental features.
        JsRuntimeAttributeEnableExperimentalFeatures = 0x00000020,
        /// Calling JsSetException will also dispatch the exception to the script debugger
        /// (if any) giving the debugger a chance to break on the exception.
        JsRuntimeAttributeDispatchSetExceptionsToDebugger = 0x00000040,
        /// Disable Failfast fatal error on OOM
        JsRuntimeAttributeDisableFatalOnOOM = 0x00000080,
        /// Runtime will not allocate executable code pages.
        /// This also implies that Native Code generation will be turned off.
        /// Note that this will break JavaScript stack decoding in tools
        /// like WPA since they rely on allocation of unique thunks to
        /// interpret each function and allocation of those thunks will be
        /// disabled as well.
        JsRuntimeAttributeDisableExecutablePageAllocation = 0x00000100,
        /// Runtime will generate bytecode buffer by treating current file as library file.
        JsRuntimeAttributeSerializeLibraryByteCode = 0x8000000,
    }
}

#[derive(thiserror::Error, Debug)]
pub enum JsError {
    /// Category of errors that relates to incorrect usage of the API itself.
    #[error("JsErrorCategoryUsage")]
    JsErrorCategoryUsage,
    /// An argument to a hosting API was invalid.
    #[error("JsErrorInvalidArgument")]
    JsErrorInvalidArgument,
    /// An argument to a hosting API was null in a context where null is not allowed.
    #[error("JsErrorNullArgument")]
    JsErrorNullArgument,
    /// The hosting API requires that a context be current, but there is no current context.
    #[error("JsErrorNoCurrentContext")]
    JsErrorNoCurrentContext,
    /// The engine is in an exception state and no APIs can be called until the exception is
    /// cleared.
    #[error("JsErrorInExceptionState")]
    JsErrorInExceptionState,
    /// A hosting API is not yet implemented.
    #[error("JsErrorNotImplemented")]
    JsErrorNotImplemented,
    /// A hosting API was called on the wrong thread.
    #[error("JsErrorWrongThread")]
    JsErrorWrongThread,
    /// A runtime that is still in use cannot be disposed.
    #[error("JsErrorRuntimeInUse")]
    JsErrorRuntimeInUse,
    /// A bad serialized script was used, or the serialized script was serialized by a different
    /// version of the Chakra engine.
    #[error("JsErrorBadSerializedScript")]
    JsErrorBadSerializedScript,
    /// The runtime is in a disabled state.
    #[error("JsErrorInDisabledState")]
    JsErrorInDisabledState,
    /// Runtime does not support reliable script interruption.
    #[error("JsErrorCannotDisableExecution")]
    JsErrorCannotDisableExecution,
    /// A heap enumeration is currently underway in the script context.
    #[error("JsErrorHeapEnumInProgress")]
    JsErrorHeapEnumInProgress,
    /// A hosting API that operates on object values was called with a non-object value.
    #[error("JsErrorArgumentNotObject")]
    JsErrorArgumentNotObject,
    /// A script context is in the middle of a profile callback.
    #[error("JsErrorInProfileCallback")]
    JsErrorInProfileCallback,
    /// A thread service callback is currently underway.
    #[error("JsErrorInThreadServiceCallback")]
    JsErrorInThreadServiceCallback,
    /// Scripts cannot be serialized in debug contexts.
    #[error("JsErrorCannotSerializeDebugScript")]
    JsErrorCannotSerializeDebugScript,
    /// The context cannot be put into a debug state because it is already in a debug state.
    #[error("JsErrorAlreadyDebuggingContext")]
    JsErrorAlreadyDebuggingContext,
    /// The context cannot start profiling because it is already profiling.
    #[error("JsErrorAlreadyProfilingContext")]
    JsErrorAlreadyProfilingContext,
    /// Idle notification given when the host did not enable idle processing.
    #[error("JsErrorIdleNotEnabled")]
    JsErrorIdleNotEnabled,
    /// The context did not accept the enqueue callback.
    #[error("JsCannotSetProjectionEnqueueCallback")]
    JsCannotSetProjectionEnqueueCallback,
    /// Failed to start projection.
    #[error("JsErrorCannotStartProjection")]
    JsErrorCannotStartProjection,
    /// The operation is not supported in an object before collect callback.
    #[error("JsErrorInObjectBeforeCollectCallback")]
    JsErrorInObjectBeforeCollectCallback,
    /// Object cannot be unwrapped to IInspectable pointer.
    #[error("JsErrorObjectNotInspectable")]
    JsErrorObjectNotInspectable,
    /// A hosting API that operates on symbol property ids but was called with a non-symbol
    /// property id. The error code is returned by JsGetSymbolFromPropertyId if the function is
    /// called with non-symbol property id.
    #[error("JsErrorPropertyNotSymbol")]
    JsErrorPropertyNotSymbol,
    /// A hosting API that operates on string property ids but was called with a non-string
    /// property id. The error code is returned by existing JsGetPropertyNamefromId if the
    /// function is called with non-string property id.
    #[error("JsErrorPropertyNotString")]
    JsErrorPropertyNotString,
    /// Module evaluation is called in wrong context.
    #[error("JsErrorInvalidContext")]
    JsErrorInvalidContext,
    /// The Module HostInfoKind provided was invalid.
    #[error("JsInvalidModuleHostInfoKind")]
    JsInvalidModuleHostInfoKind,
    /// Module was parsed already when JsParseModuleSource is called.
    #[error("JsErrorModuleParsed")]
    JsErrorModuleParsed,
    /// Argument passed to JsCreateWeakReference is a primitive that is not managed by the GC.
    /// No weak reference is required, the value will never be collected.
    #[error("JsNoWeakRefRequired")]
    JsNoWeakRefRequired,
    /// The `Promise` object is still in the pending state.
    #[error("JsErrorPromisePending")]
    JsErrorPromisePending,
    /// Module was not yet evaluated when JsGetModuleNamespace was called.
    #[error("JsErrorModuleNotEvaluated")]
    JsErrorModuleNotEvaluated,
    /// Category of errors that relates to errors occurring within the engine itself.
    #[error("JsErrorCategoryEngine")]
    JsErrorCategoryEngine,
    /// The Chakra engine has run out of memory.
    #[error("JsErrorOutOfMemory")]
    JsErrorOutOfMemory,
    /// The Chakra engine failed to set the Floating Point Unit state.
    #[error("JsErrorBadFPUState")]
    JsErrorBadFPUState,
    /// Category of errors that relates to errors in a script.
    #[error("JsErrorCategoryScript")]
    JsErrorCategoryScript,
    /// A JavaScript exception occurred while running a script.
    #[error("JsErrorScriptException")]
    JsErrorScriptException,
    /// JavaScript failed to compile.
    #[error("JsErrorScriptCompile")]
    JsErrorScriptCompile,
    /// A script was terminated due to a request to suspend a runtime.
    #[error("JsErrorScriptTerminated")]
    JsErrorScriptTerminated,
    /// A script was terminated because it tried to use `eval` or `function` and eval
    /// was disabled.
    #[error("JsErrorScriptEvalDisabled")]
    JsErrorScriptEvalDisabled,
    /// Category of errors that are fatal and signify failure of the engine.
    #[error("JsErrorCategoryFatal")]
    JsErrorCategoryFatal,
    /// A fatal error in the engine has occurred.
    #[error("JsErrorFatal")]
    JsErrorFatal,
    /// A hosting API was called with object created on different javascript runtime.
    #[error("JsErrorWrongRuntime")]
    JsErrorWrongRuntime,
    /// Category of errors that are related to failures during diagnostic operations.
    #[error("JsErrorCategoryDiagError")]
    JsErrorCategoryDiagError,
    /// The object for which the debugging API was called was not found
    #[error("JsErrorDiagAlreadyInDebugMode")]
    JsErrorDiagAlreadyInDebugMode,
    /// The debugging API can only be called when VM is in debug mode
    #[error("JsErrorDiagNotInDebugMode")]
    JsErrorDiagNotInDebugMode,
    /// The debugging API can only be called when VM is at a break
    #[error("JsErrorDiagNotAtBreak")]
    JsErrorDiagNotAtBreak,
    /// Debugging API was called with an invalid handle.
    #[error("JsErrorDiagInvalidHandle")]
    JsErrorDiagInvalidHandle,
    /// The object for which the debugging API was called was not found
    #[error("JsErrorDiagObjectNotFound")]
    JsErrorDiagObjectNotFound,
    /// VM was unable to perform the request action
    #[error("JsErrorDiagUnableToPerformAction")]
    JsErrorDiagUnableToPerformAction,
    /// Serializer/Deserializer does not support current data
    #[error("JsSerializerNotSupported")]
    JsSerializerNotSupported,
    /// Current object is not transferable during serialization
    #[error("JsTransferableNotSupported")]
    JsTransferableNotSupported,
    /// Current object is already detached when serialized
    #[error("JsTransferableAlreadyDetached")]
    JsTransferableAlreadyDetached,
}

pub trait JsErrorExt {
    fn as_result(&self) -> Result<(), JsError>;
}

impl JsErrorExt for JsErrorCode {
    fn as_result(&self) -> Result<(), JsError> {
        match *self {
            JsErrorCode::JsNoError => Ok(()),
            JsErrorCode::JsErrorCategoryUsage => Err(JsError::JsErrorCategoryUsage),
            JsErrorCode::JsErrorInvalidArgument => Err(JsError::JsErrorInvalidArgument),
            JsErrorCode::JsErrorNullArgument => Err(JsError::JsErrorNullArgument),
            JsErrorCode::JsErrorNoCurrentContext => Err(JsError::JsErrorNoCurrentContext),
            JsErrorCode::JsErrorInExceptionState => Err(JsError::JsErrorInExceptionState),
            JsErrorCode::JsErrorNotImplemented => Err(JsError::JsErrorNotImplemented),
            JsErrorCode::JsErrorWrongThread => Err(JsError::JsErrorWrongThread),
            JsErrorCode::JsErrorRuntimeInUse => Err(JsError::JsErrorRuntimeInUse),
            JsErrorCode::JsErrorBadSerializedScript => Err(JsError::JsErrorBadSerializedScript),
            JsErrorCode::JsErrorInDisabledState => Err(JsError::JsErrorInDisabledState),
            JsErrorCode::JsErrorCannotDisableExecution => {
                Err(JsError::JsErrorCannotDisableExecution)
            }
            JsErrorCode::JsErrorHeapEnumInProgress => Err(JsError::JsErrorHeapEnumInProgress),
            JsErrorCode::JsErrorArgumentNotObject => Err(JsError::JsErrorArgumentNotObject),
            JsErrorCode::JsErrorInProfileCallback => Err(JsError::JsErrorInProfileCallback),
            JsErrorCode::JsErrorInThreadServiceCallback => {
                Err(JsError::JsErrorInThreadServiceCallback)
            }
            JsErrorCode::JsErrorCannotSerializeDebugScript => {
                Err(JsError::JsErrorCannotSerializeDebugScript)
            }
            JsErrorCode::JsErrorAlreadyDebuggingContext => {
                Err(JsError::JsErrorAlreadyDebuggingContext)
            }
            JsErrorCode::JsErrorAlreadyProfilingContext => {
                Err(JsError::JsErrorAlreadyProfilingContext)
            }
            JsErrorCode::JsErrorIdleNotEnabled => Err(JsError::JsErrorIdleNotEnabled),
            JsErrorCode::JsCannotSetProjectionEnqueueCallback => {
                Err(JsError::JsCannotSetProjectionEnqueueCallback)
            }
            JsErrorCode::JsErrorCannotStartProjection => Err(JsError::JsErrorCannotStartProjection),
            JsErrorCode::JsErrorInObjectBeforeCollectCallback => {
                Err(JsError::JsErrorInObjectBeforeCollectCallback)
            }
            JsErrorCode::JsErrorObjectNotInspectable => Err(JsError::JsErrorObjectNotInspectable),
            JsErrorCode::JsErrorPropertyNotSymbol => Err(JsError::JsErrorPropertyNotSymbol),
            JsErrorCode::JsErrorPropertyNotString => Err(JsError::JsErrorPropertyNotString),
            JsErrorCode::JsErrorInvalidContext => Err(JsError::JsErrorInvalidContext),
            JsErrorCode::JsInvalidModuleHostInfoKind => Err(JsError::JsInvalidModuleHostInfoKind),
            JsErrorCode::JsErrorModuleParsed => Err(JsError::JsErrorModuleParsed),
            JsErrorCode::JsNoWeakRefRequired => Err(JsError::JsNoWeakRefRequired),
            JsErrorCode::JsErrorPromisePending => Err(JsError::JsErrorPromisePending),
            JsErrorCode::JsErrorModuleNotEvaluated => Err(JsError::JsErrorModuleNotEvaluated),
            JsErrorCode::JsErrorCategoryEngine => Err(JsError::JsErrorCategoryEngine),
            JsErrorCode::JsErrorOutOfMemory => Err(JsError::JsErrorOutOfMemory),
            JsErrorCode::JsErrorBadFPUState => Err(JsError::JsErrorBadFPUState),
            JsErrorCode::JsErrorCategoryScript => Err(JsError::JsErrorCategoryScript),
            JsErrorCode::JsErrorScriptException => Err(JsError::JsErrorScriptException),
            JsErrorCode::JsErrorScriptCompile => Err(JsError::JsErrorScriptCompile),
            JsErrorCode::JsErrorScriptTerminated => Err(JsError::JsErrorScriptTerminated),
            JsErrorCode::JsErrorScriptEvalDisabled => Err(JsError::JsErrorScriptEvalDisabled),
            JsErrorCode::JsErrorCategoryFatal => Err(JsError::JsErrorCategoryFatal),
            JsErrorCode::JsErrorFatal => Err(JsError::JsErrorFatal),
            JsErrorCode::JsErrorWrongRuntime => Err(JsError::JsErrorWrongRuntime),
            JsErrorCode::JsErrorCategoryDiagError => Err(JsError::JsErrorCategoryDiagError),
            JsErrorCode::JsErrorDiagAlreadyInDebugMode => {
                Err(JsError::JsErrorDiagAlreadyInDebugMode)
            }
            JsErrorCode::JsErrorDiagNotInDebugMode => Err(JsError::JsErrorDiagNotInDebugMode),
            JsErrorCode::JsErrorDiagNotAtBreak => Err(JsError::JsErrorDiagNotAtBreak),
            JsErrorCode::JsErrorDiagInvalidHandle => Err(JsError::JsErrorDiagInvalidHandle),
            JsErrorCode::JsErrorDiagObjectNotFound => Err(JsError::JsErrorDiagObjectNotFound),
            JsErrorCode::JsErrorDiagUnableToPerformAction => {
                Err(JsError::JsErrorDiagUnableToPerformAction)
            }
            JsErrorCode::JsSerializerNotSupported => Err(JsError::JsSerializerNotSupported),
            JsErrorCode::JsTransferableNotSupported => Err(JsError::JsTransferableNotSupported),
            JsErrorCode::JsTransferableAlreadyDetached => {
                Err(JsError::JsTransferableAlreadyDetached)
            }
            _ => unimplemented!(),
        }
    }
}

#[cfg(test)]
mod tests {
    use crate::rt_interface::JsRuntimeHandle;
    use crate::rt_interface::ffi::{ChakraRTInterface, JsErrorCode, JsRuntimeAttributes};

    #[test]
    fn create_and_dispose_runtime() {
        let mut runtime = JsRuntimeHandle::default();
        unsafe {
            let res = ChakraRTInterface::JsCreateRuntime(
                JsRuntimeAttributes::JsRuntimeAttributeNone,
                &raw mut runtime,
            );
            assert_eq!(res, JsErrorCode::JsNoError);

            assert!(!runtime.is_invalid());

            let res = ChakraRTInterface::JsDisposeRuntime(runtime);
            assert_eq!(res, JsErrorCode::JsNoError);
        }
    }
}
