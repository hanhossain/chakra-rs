pub use ffi::{ConfigContext, CoreConfig, HostConfig};

#[cxx::bridge(namespace = "chakra_rs")]
mod ffi {
    #[derive(Debug, Clone, Default, Serialize, Deserialize)]
    struct ConfigContext {
        host: HostConfig,
        core: CoreConfig,
        host_args: Vec<String>,
    }

    #[derive(Debug, Clone, Default, Serialize, Deserialize)]
    struct HostConfig {
        /// If source is UTF8, deserializes from bytecode file
        serialized: bool,
        /// Create parser state cache while parsing and use it during script execution
        use_parser_state_cache: bool,
        /// load the script as a module
        module: bool,
        /// Don't return error code on script error
        ignore_script_error_code: bool,
        /// Mute host error output, e.g. module load failures
        mute_host_error_msg: bool,
        /// Output traces for host callbacks
        trace_host_callback: bool,
        /// load Test262 harness
        test262: bool,
    }

    #[derive(Debug, Clone, Default, Serialize, Deserialize)]
    struct CoreConfig {
        filename: String,
        args: Vec<String>,
    }
}
