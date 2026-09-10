#[cxx::bridge(namespace = "chakra_rs::config")]
mod ffi {
    #[derive(Debug, Clone, Default, Serialize, Deserialize)]
    struct CoreConfig {
        filename: String,
        serialized: bool,
        args: Vec<String>,
        host_args: Vec<String>,
    }
}

pub use ffi::CoreConfig;
