pub use ffi::TestHooks;
use std::collections::HashMap;
use std::sync::{LazyLock, RwLock};

static STORE: LazyLock<RwLock<HashMap<String, String>>> =
    LazyLock::new(|| RwLock::new(HashMap::new()));

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("TestHooks.h");
        type TestHooks;

        #[Self = "TestHooks"]
        fn SetConfigFlags(vargs: &Vec<String>) -> i32;
    }

    #[namespace = "chakra_rs::helpers"]
    extern "Rust" {
        type ScriptCache;

        #[Self = "ScriptCache"]
        fn load_script_from_file(filename: &str) -> Result<String>;

        #[Self = "ScriptCache"]
        fn load_script_with_full_path(filename: &str, full_path: &str) -> Result<String>;
    }
}

pub struct ScriptCache;

impl ScriptCache {
    #[tracing::instrument(err)]
    pub fn load_script_from_file(filename: &str) -> std::io::Result<String> {
        let store = STORE.read().unwrap();
        if let Some(entry) = store.get(filename) {
            return Ok(entry.clone());
        }

        crate::filesystem::file_to_string(filename)
    }

    #[tracing::instrument(err)]
    fn load_script_with_full_path(filename: &str, full_path: &str) -> std::io::Result<String> {
        // TODO: This is a port of the original code. Instead, we should see if this works if we search
        //  full_path first then filename.
        let store = STORE.read().unwrap();
        if let Some(entry) = store.get(filename).or_else(|| store.get(full_path)) {
            return Ok(entry.clone());
        }

        crate::filesystem::file_to_string(full_path)
    }

    pub fn add_script(filename: String, data: String) {
        let mut store = STORE.write().unwrap();
        store.insert(filename, data);
    }
}
