use crate::jsrt::JsError;
pub use bridge::{
    CVoid, JsErrorCode, JsModuleHostInfoKind, JsNativeFunctionArgs, JsParseScriptAttributes,
    JsRuntimeAttributes,
};
use std::ffi::c_void;

#[repr(transparent)]
#[derive(Copy, Clone, Default)]
pub struct JsRuntimeHandle(*mut c_void);

unsafe impl cxx::ExternType for JsRuntimeHandle {
    type Id = cxx::type_id!("JsRuntimeHandle");
    type Kind = cxx::kind::Trivial;
}

impl JsRuntimeHandle {
    pub fn is_invalid(&self) -> bool {
        self.0 == std::ptr::null_mut()
    }
}

#[repr(transparent)]
#[derive(Default)]
pub struct JsContextRef(*mut c_void);

unsafe impl cxx::ExternType for JsContextRef {
    type Id = cxx::type_id!("JsContextRef");
    type Kind = cxx::kind::Trivial;
}

#[repr(transparent)]
#[derive(Default, Clone)]
pub struct JsValueRef(*mut c_void);

unsafe impl cxx::ExternType for JsValueRef {
    type Id = cxx::type_id!("JsValueRef");
    type Kind = cxx::kind::Trivial;
}

impl JsValueRef {
    pub fn is_null(&self) -> bool {
        self.0.is_null()
    }

    #[tracing::instrument(level = "trace", skip_all, err)]
    pub fn to_string(&self) -> Result<String, JsError> {
        let mut s = String::new();
        bridge::JsToString(self.as_ref(), &mut s).as_result()?;
        Ok(s)
    }
}

impl AsRef<JsValueRef> for JsValueRef {
    fn as_ref(&self) -> &JsValueRef {
        &self
    }
}

#[repr(transparent)]
#[derive(Default)]
pub struct JsPropertyIdRef(*mut c_void);

impl JsPropertyIdRef {
    pub fn is_null(&self) -> bool {
        self.0.is_null()
    }
}

unsafe impl cxx::ExternType for JsPropertyIdRef {
    type Id = cxx::type_id!("JsPropertyIdRef");
    type Kind = cxx::kind::Trivial;
}

#[repr(transparent)]
pub struct JsSourceContext(pub usize);

unsafe impl cxx::ExternType for JsSourceContext {
    type Id = cxx::type_id!("JsSourceContext");
    type Kind = cxx::kind::Trivial;
}

#[repr(transparent)]
#[derive(Default, Clone, Eq, PartialEq, Hash)]
pub struct JsModuleRecord(*mut CVoid);

unsafe impl cxx::ExternType for JsModuleRecord {
    type Id = cxx::type_id!("JsModuleRecord");
    type Kind = cxx::kind::Trivial;
}

unsafe impl Send for JsModuleRecord {}
unsafe impl Sync for JsModuleRecord {}

#[repr(transparent)]
pub struct CULong(pub std::ffi::c_ulong);

unsafe impl cxx::ExternType for CULong {
    type Id = cxx::type_id!("CULong");
    type Kind = cxx::kind::Trivial;
}

#[derive(Default)]
#[repr(transparent)]
pub struct JsSharedArrayBufferContentHandle(*mut CVoid);

unsafe impl cxx::ExternType for JsSharedArrayBufferContentHandle {
    type Id = cxx::type_id!("JsSharedArrayBufferContentHandle");
    type Kind = cxx::kind::Trivial;
}

#[cxx::bridge]
pub(super) mod bridge {
    extern "C++" {
        include!("ChakraCore.h");

        type CVoid;

        type JsRuntimeHandle = super::JsRuntimeHandle;
        type JsContextRef = super::JsContextRef;
        type JsValueRef = super::JsValueRef;
        type JsPropertyIdRef = super::JsPropertyIdRef;
        type JsSourceContext = super::JsSourceContext;
        type JsModuleRecord = super::JsModuleRecord;
        type CULong = super::CULong;
        type JsSharedArrayBufferContentHandle = super::JsSharedArrayBufferContentHandle;

        type JsErrorCode;
        type JsRuntimeAttributes;
        type JsParseScriptAttributes;
        type JsModuleHostInfoKind;
    }

    #[namespace = "chakracore::jsrt"]
    unsafe extern "C++" {
        unsafe fn JsCreateString(content: &str, value: *mut JsValueRef) -> JsErrorCode;
        unsafe fn JsCreateObject(object: *mut JsValueRef) -> JsErrorCode;
        unsafe fn JsCreatePropertyId(name: &str, object: *mut JsPropertyIdRef) -> JsErrorCode;
        unsafe fn JsCreateArray(length: u32, array: *mut JsValueRef) -> JsErrorCode;
        unsafe fn JsCreateError(message: JsValueRef, error: *mut JsValueRef) -> JsErrorCode;
        fn JsToString(value: &JsValueRef, string: &mut String) -> JsErrorCode;
        fn JsSetProperty(
            object: JsValueRef,
            property: JsPropertyIdRef,
            value: JsValueRef,
            useStrictRules: bool,
        ) -> JsErrorCode;
        fn JsSetIndexedProperty(
            object: JsValueRef,
            index: JsValueRef,
            value: JsValueRef,
        ) -> JsErrorCode;
        unsafe fn JsIntToNumber(int_value: i32, value: *mut JsValueRef) -> JsErrorCode;
        unsafe fn JsDoubleToNumber(double_value: f64, value: *mut JsValueRef) -> JsErrorCode;
        unsafe fn JsNumberToInt(js_number: JsValueRef, value: *mut i32) -> JsErrorCode;
        unsafe fn JsNumberToDouble(js_number: JsValueRef, value: *mut f64) -> JsErrorCode;
        unsafe fn JsGetGlobalObject(global_object: *mut JsValueRef) -> JsErrorCode;
        unsafe fn JsGetUndefinedValue(value: *mut JsValueRef) -> JsErrorCode;

        unsafe fn JsCreateNamedFunction(
            name: &JsValueRef,
            native_function: unsafe fn(
                JsValueRef,
                bool,
                *mut JsValueRef,
                u16,
                *mut CVoid,
            ) -> JsValueRef,
            callback_state: *mut CVoid,
            function: *mut JsValueRef,
        ) -> JsErrorCode;

        fn JsSetException(exception: JsValueRef) -> JsErrorCode;
        unsafe fn JsHasException(has_exception: *mut bool) -> JsErrorCode;
        unsafe fn JsGetModuleNamespace(
            request_module: JsModuleRecord,
            module_namespace: *mut JsValueRef,
        ) -> JsErrorCode;
    }

    impl CxxVector<JsValueRef> {}

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

    #[derive(Debug)]
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

    #[repr(i32)]
    enum JsParseScriptAttributes {
        /// Default attribute
        JsParseScriptAttributeNone = 0x0,
        /// Specified script is internal and non-user code. Hidden from debugger
        JsParseScriptAttributeLibraryCode = 0x1,
        /// ChakraCore assumes ExternalArrayBuffer is Utf8 by default.
        /// This one needs to be set for Utf16
        JsParseScriptAttributeArrayBufferIsUtf16Encoded = 0x2,
        /// Script should be parsed in strict mode
        JsParseScriptAttributeStrictMode = 0x4,
    }

    #[repr(i32)]
    enum JsModuleHostInfoKind {
        /// An exception object - e.g. if the module file cannot be found.
        JsModuleHostInfo_Exception = 0x01,
        /// Host defined info.
        JsModuleHostInfo_HostDefined = 0x02,
        /// Callback for receiving notification when module is ready.
        JsModuleHostInfo_NotifyModuleReadyCallback = 0x3,
        /// Callback for receiving notification to fetch a dependent module.
        JsModuleHostInfo_FetchImportedModuleCallback = 0x4,
        /// Callback for receiving notification for calls to ```import()```
        JsModuleHostInfo_FetchImportedModuleFromScriptCallback = 0x5,
        /// URL for use in error stack traces and debugging.
        JsModuleHostInfo_Url = 0x6,
        /// Callback to allow host to initialize import.meta object properties.
        JsModuleHostInfo_InitializeImportMetaCallback = 0x7,
        /// Callback to report module completion or exception thrown when evaluating a module.
        JsModuleHostInfo_ReportModuleCompletionCallback = 0x8,
    }

    #[namespace = "chakra_rs"]
    struct JsNativeFunctionArgs<'a> {
        /// A function object that represents the function being invoked.
        callee: JsValueRef,
        /// Indicates whether this is a regular call or a 'new' call.
        is_construct_call: bool,
        /// The arguments to the call.
        arguments: &'a [JsValueRef],
    }
}

impl JsErrorCode {
    pub fn as_result(&self) -> Result<(), JsError> {
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

impl From<JsError> for JsErrorCode {
    fn from(value: JsError) -> Self {
        match value {
            JsError::JsErrorCategoryUsage => JsErrorCode::JsErrorCategoryUsage,
            JsError::JsErrorInvalidArgument => JsErrorCode::JsErrorInvalidArgument,
            JsError::JsErrorNullArgument => JsErrorCode::JsErrorNullArgument,
            JsError::JsErrorNoCurrentContext => JsErrorCode::JsErrorNoCurrentContext,
            JsError::JsErrorInExceptionState => JsErrorCode::JsErrorInExceptionState,
            JsError::JsErrorNotImplemented => JsErrorCode::JsErrorNotImplemented,
            JsError::JsErrorWrongThread => JsErrorCode::JsErrorWrongThread,
            JsError::JsErrorRuntimeInUse => JsErrorCode::JsErrorRuntimeInUse,
            JsError::JsErrorBadSerializedScript => JsErrorCode::JsErrorBadSerializedScript,
            JsError::JsErrorInDisabledState => JsErrorCode::JsErrorInDisabledState,
            JsError::JsErrorCannotDisableExecution => JsErrorCode::JsErrorCannotDisableExecution,
            JsError::JsErrorHeapEnumInProgress => JsErrorCode::JsErrorHeapEnumInProgress,
            JsError::JsErrorArgumentNotObject => JsErrorCode::JsErrorArgumentNotObject,
            JsError::JsErrorInProfileCallback => JsErrorCode::JsErrorInProfileCallback,
            JsError::JsErrorInThreadServiceCallback => JsErrorCode::JsErrorInThreadServiceCallback,
            JsError::JsErrorCannotSerializeDebugScript => {
                JsErrorCode::JsErrorCannotSerializeDebugScript
            }
            JsError::JsErrorAlreadyDebuggingContext => JsErrorCode::JsErrorAlreadyDebuggingContext,
            JsError::JsErrorAlreadyProfilingContext => JsErrorCode::JsErrorAlreadyProfilingContext,
            JsError::JsErrorIdleNotEnabled => JsErrorCode::JsErrorIdleNotEnabled,
            JsError::JsCannotSetProjectionEnqueueCallback => {
                JsErrorCode::JsCannotSetProjectionEnqueueCallback
            }
            JsError::JsErrorCannotStartProjection => JsErrorCode::JsErrorCannotStartProjection,
            JsError::JsErrorInObjectBeforeCollectCallback => {
                JsErrorCode::JsErrorInObjectBeforeCollectCallback
            }
            JsError::JsErrorObjectNotInspectable => JsErrorCode::JsErrorObjectNotInspectable,
            JsError::JsErrorPropertyNotSymbol => JsErrorCode::JsErrorPropertyNotSymbol,
            JsError::JsErrorPropertyNotString => JsErrorCode::JsErrorPropertyNotString,
            JsError::JsErrorInvalidContext => JsErrorCode::JsErrorInvalidContext,
            JsError::JsInvalidModuleHostInfoKind => JsErrorCode::JsInvalidModuleHostInfoKind,
            JsError::JsErrorModuleParsed => JsErrorCode::JsErrorModuleParsed,
            JsError::JsNoWeakRefRequired => JsErrorCode::JsNoWeakRefRequired,
            JsError::JsErrorPromisePending => JsErrorCode::JsErrorPromisePending,
            JsError::JsErrorModuleNotEvaluated => JsErrorCode::JsErrorModuleNotEvaluated,
            JsError::JsErrorCategoryEngine => JsErrorCode::JsErrorCategoryEngine,
            JsError::JsErrorOutOfMemory => JsErrorCode::JsErrorOutOfMemory,
            JsError::JsErrorBadFPUState => JsErrorCode::JsErrorBadFPUState,
            JsError::JsErrorCategoryScript => JsErrorCode::JsErrorCategoryScript,
            JsError::JsErrorScriptException => JsErrorCode::JsErrorScriptException,
            JsError::JsErrorScriptCompile => JsErrorCode::JsErrorScriptCompile,
            JsError::JsErrorScriptTerminated => JsErrorCode::JsErrorScriptTerminated,
            JsError::JsErrorScriptEvalDisabled => JsErrorCode::JsErrorScriptEvalDisabled,
            JsError::JsErrorCategoryFatal => JsErrorCode::JsErrorCategoryFatal,
            JsError::JsErrorFatal => JsErrorCode::JsErrorFatal,
            JsError::JsErrorWrongRuntime => JsErrorCode::JsErrorWrongRuntime,
            JsError::JsErrorCategoryDiagError => JsErrorCode::JsErrorCategoryDiagError,
            JsError::JsErrorDiagAlreadyInDebugMode => JsErrorCode::JsErrorDiagAlreadyInDebugMode,
            JsError::JsErrorDiagNotInDebugMode => JsErrorCode::JsErrorDiagNotInDebugMode,
            JsError::JsErrorDiagNotAtBreak => JsErrorCode::JsErrorDiagNotAtBreak,
            JsError::JsErrorDiagInvalidHandle => JsErrorCode::JsErrorDiagInvalidHandle,
            JsError::JsErrorDiagObjectNotFound => JsErrorCode::JsErrorDiagObjectNotFound,
            JsError::JsErrorDiagUnableToPerformAction => {
                JsErrorCode::JsErrorDiagUnableToPerformAction
            }
            JsError::JsSerializerNotSupported => JsErrorCode::JsSerializerNotSupported,
            JsError::JsTransferableNotSupported => JsErrorCode::JsTransferableNotSupported,
            JsError::JsTransferableAlreadyDetached => JsErrorCode::JsTransferableAlreadyDetached,
        }
    }
}
