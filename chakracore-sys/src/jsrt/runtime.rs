use crate::jsrt::ffi::bridge;
use crate::jsrt::ffi::bridge::{AllocationPolicyManager, FfiThreadContext, ThreadContextFlags};
use crate::jsrt::{JsContext, JsContextRef, JsError, JsRuntimeAttributes, JsrtRuntime};
use cxx::UniquePtr;
use std::pin::Pin;

pub struct JsRuntime {
    handle: UniquePtr<JsrtRuntime>,
}

impl JsRuntime {
    pub fn create(attributes: JsRuntimeAttributes) -> Result<Self, JsError> {
        bridge::VALIDATE_ENTER_CURRENT_THREAD();

        let disable_background_work =
            attributes.contains(JsRuntimeAttributes::JsRuntimeAttributeDisableBackgroundWork);
        let policy_manager = AllocationPolicyManager::FfiNew(!disable_background_work);

        unsafe {
            let mut thread_context: *mut FfiThreadContext = std::ptr::null_mut();
            bridge::JsCreateRuntime(
                attributes,
                policy_manager,
                &raw mut thread_context as *mut _,
            )
            .as_result()?;
            {
                let mut thread_context = Pin::new_unchecked(&mut *thread_context);

                if attributes.contains(JsRuntimeAttributes::JsRuntimeAttributeDisableBackgroundWork)
                {
                    thread_context.as_mut().OptimizeForManyInstances(true);
                    // TODO (hanhossain): this should follow ENABLE_NATIVE_CODEGEN
                    #[cfg(target_arch = "x86_64")]
                    thread_context.as_mut().EnableBgJit(false);
                }

                if attributes.contains(JsRuntimeAttributes::JsRuntimeAttributeAllowScriptInterrupt)
                {
                    thread_context.as_mut().SetThreadContextFlag(
                        ThreadContextFlags::ThreadContextFlagCanDisableExecution,
                    );
                }

                if attributes.contains(JsRuntimeAttributes::JsRuntimeAttributeDisableEval) {
                    thread_context
                        .as_mut()
                        .SetThreadContextFlag(ThreadContextFlags::ThreadContextFlagEvalDisabled);
                }

                if attributes
                    .contains(JsRuntimeAttributes::JsRuntimeAttributeDisableNativeCodeGeneration)
                {
                    thread_context
                        .as_mut()
                        .SetThreadContextFlag(ThreadContextFlags::ThreadContextFlagNoJIT);
                }

                if attributes.contains(
                    JsRuntimeAttributes::JsRuntimeAttributeDisableExecutablePageAllocation,
                ) {
                    thread_context.as_mut().SetThreadContextFlag(
                        ThreadContextFlags::ThreadContextFlagNoJIT
                            | ThreadContextFlags::ThreadContextFlagNoDynamicThunks,
                    );
                }

                if attributes.contains(JsRuntimeAttributes::JsRuntimeAttributeDisableFatalOnOOM) {
                    thread_context.as_mut().SetThreadContextFlag(
                        ThreadContextFlags::ThreadContextFlagDisableFatalOnOOM,
                    );
                }
            }

            let handle = JsrtRuntime::FfiNew(thread_context);

            Ok(Self { handle })
        }
    }

    pub fn create_context(&self) -> Result<JsContext, JsError> {
        let mut context = JsContextRef::default();
        unsafe {
            bridge::JsCreateContext(self.handle.as_mut_ptr(), &raw mut context).as_result()?;
        }
        Ok(JsContext(context))
    }
}
