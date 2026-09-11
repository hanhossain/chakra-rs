#[cxx::bridge(namespace = "chakra_rs")]
mod ffi {
    #[derive(Debug, Clone, Default, Serialize, Deserialize)]
    struct ConfigContext {
        host: HostConfig,
        core: CoreConfig,
    }

    #[derive(Debug, Clone, Default, Serialize, Deserialize)]
    struct HostConfig {
        /// If source is UTF8, deserializes from bytecode file
        serialized: bool,
        /// Create parser state cache while parsing and use it during script execution
        use_parser_state_cache: bool,
        /// load the script as a module
        module: bool,
    }

    #[derive(Debug, Clone, Default, Serialize, Deserialize)]
    struct CoreConfig {
        filename: String,
        args: Vec<String>,
        host_args: Vec<String>,
    }
}

pub use ffi::{ConfigContext, CoreConfig, HostConfig};
