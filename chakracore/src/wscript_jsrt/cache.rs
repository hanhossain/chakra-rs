use std::collections::HashMap;
use std::sync::RwLock;

pub struct ScriptCache {
    store: RwLock<HashMap<String, String>>,
}

impl ScriptCache {
    pub fn new() -> Self {
        Self {
            store: RwLock::default(),
        }
    }

    #[tracing::instrument(skip(self), err)]
    pub fn get_script(&self, filename: &str) -> std::io::Result<String> {
        let store = self.store.read().unwrap();
        if let Some(entry) = store.get(filename) {
            return Ok(entry.clone());
        }

        crate::filesystem::file_to_string(filename)
    }

    #[tracing::instrument(skip(self), err)]
    pub fn get_script_with_full_path(
        &self,
        filename: &str,
        full_path: &str,
    ) -> std::io::Result<String> {
        // TODO: This is a port of the original code. Instead, we should see if this works if we search
        //  full_path first then filename.
        let store = self.store.read().unwrap();
        if let Some(entry) = store.get(filename).or_else(|| store.get(full_path)) {
            return Ok(entry.clone());
        }

        crate::filesystem::file_to_string(full_path)
    }

    pub fn add_script(&self, filename: String, data: String) {
        let mut store = self.store.write().unwrap();
        store.insert(filename, data);
    }
}
