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
