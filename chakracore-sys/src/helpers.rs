pub use ffi::{Helpers, SourceMap};

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("Helpers.h");

        type Helpers;
    }

    unsafe extern "C++" {
        include!("SourceMap.h");
        type SourceMap;
        #[Self = "SourceMap"]
        fn Find(filename: &str) -> SharedPtr<CxxString>;
    }

    #[namespace = "chakra_rs::helpers"]
    extern "Rust" {
        fn load_script_from_file(filename: &str) -> Result<String>;
        fn load_script_with_full_path(filename: &str, full_path: &str) -> Result<String>;
    }
}

#[tracing::instrument(err)]
pub fn load_script_from_file(filename: &str) -> std::io::Result<String> {
    let entry = SourceMap::Find(filename);
    if !entry.is_null() {
        Ok(entry.to_string())
    } else {
        crate::filesystem::file_to_string(filename)
    }
}

#[tracing::instrument(err)]
fn load_script_with_full_path(filename: &str, full_path: &str) -> std::io::Result<String> {
    // TODO: This is a port of the original code. Instead, we should see if this works if we search
    //  full_path first then filename.
    let entry = SourceMap::Find(filename);
    if !entry.is_null() {
        return Ok(entry.to_string());
    }

    let entry = SourceMap::Find(full_path);
    if !entry.is_null() {
        return Ok(entry.to_string());
    }

    crate::filesystem::file_to_string(full_path)
}
