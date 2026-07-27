#[cxx::bridge(namespace = "chakra_rs::log")]
mod ffi {
    extern "Rust" {
        fn error(function_name: &str, file_name: &str, line: u32, message: &str);
    }
}

fn error(function_name: &str, file_name: &str, line: u32, message: &str) {
    tracing::error!(function_name, file_name, line, message);
}
