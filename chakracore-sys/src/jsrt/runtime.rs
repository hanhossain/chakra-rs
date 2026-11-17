use crate::jsrt::ffi::bridge;
use crate::jsrt::ffi::bridge::AllocationPolicyManager;
use crate::jsrt::{JsContext, JsContextRef, JsError, JsRuntimeAttributes, JsrtRuntime};
use cxx::UniquePtr;

pub struct JsRuntime {
    handle: UniquePtr<JsrtRuntime>,
}

impl JsRuntime {
    pub fn create(attributes: JsRuntimeAttributes) -> Result<Self, JsError> {
        bridge::VALIDATE_ENTER_CURRENT_THREAD();

        let disable_background_work =
            attributes.contains(JsRuntimeAttributes::JsRuntimeAttributeDisableBackgroundWork);
        let policy_manager = AllocationPolicyManager::FfiNew(!disable_background_work);

        let mut handle = UniquePtr::null();
        bridge::JsCreateRuntime(attributes, &mut handle, policy_manager).as_result()?;
        Ok(Self { handle })
    }

    pub fn create_context(&self) -> Result<JsContext, JsError> {
        let mut context = JsContextRef::default();
        unsafe {
            bridge::JsCreateContext(self.handle.as_mut_ptr(), &raw mut context).as_result()?;
        }
        Ok(JsContext(context))
    }
}
