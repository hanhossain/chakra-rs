#[cxx::bridge(namespace = "chakra_rs::config")]
mod ffi {
    #[derive(Debug, Clone, Default, Serialize, Deserialize)]
    struct CoreConfig {
        filename: String,
        args: Vec<String>,
    }
}

pub use ffi::CoreConfig;
