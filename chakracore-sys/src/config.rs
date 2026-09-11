#[cxx::bridge(namespace = "chakra_rs::config")]
mod ffi {
    #[derive(Debug, Clone, Default, Serialize, Deserialize)]
    struct CoreConfig {
        filename: String,
        serialized: bool,
        args: Vec<String>,
        host_args: Vec<String>,
        use_parser_state_cache: bool,
        module: bool,
    }
}

pub use ffi::CoreConfig;
