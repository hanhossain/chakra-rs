use crate::wscript_jsrt::ffi::RuntimeThreadData;
use cxx::SharedPtr;

pub(super) struct ThreadData {
    pub(super) inner: SharedPtr<RuntimeThreadData>,
}

impl ThreadData {
    pub(super) fn new() -> Self {
        let inner = RuntimeThreadData::New();
        Self { inner }
    }

    pub(super) fn new_with_initial_source(source: &str) -> Self {
        let inner = RuntimeThreadData::NewWithInitialSource(&source.to_owned());
        Self { inner }
    }
}
