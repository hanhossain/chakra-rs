pub use ffi::{CloseHandle, CreateThread, WaitForMultipleObjects};

#[repr(transparent)]
#[derive(Default, Clone)]
pub struct Handle(*mut std::ffi::c_void);

impl Handle {
    pub fn is_null(&self) -> bool {
        self.0.is_null()
    }
}

unsafe impl cxx::ExternType for Handle {
    type Id = cxx::type_id!("HANDLE");
    type Kind = cxx::kind::Trivial;
}

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("pal_ffi/pal_thread_ffi.h");
        include!("chakracore-sys/include/ffi_types.h");

        type CVoid = crate::jsrt::CVoid;
        #[cxx_name = "HANDLE"]
        type Handle = super::Handle;
        #[namespace = "pal_ffi"]
        unsafe fn CreateThread(
            func: unsafe fn(*mut CVoid, Handle) -> u32,
            param: *mut CVoid,
            creation_flags: u32,
        ) -> Handle;
        #[namespace = "pal_ffi"]
        fn WaitForMultipleObjects(handles: &[Handle], wait_all: bool) -> u32;
        #[namespace = "pal_ffi"]
        fn CloseHandle(handle: &Handle) -> bool;
    }
}
