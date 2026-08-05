#[cxx::bridge(namespace = "chakra_rs::log")]
mod ffi {
    extern "Rust" {
        fn error(function_name: &str, file_name: &str, line: u32, message: &str);
        fn warn(function_name: &str, file_name: &str, line: u32, message: &str);
        fn info(function_name: &str, file_name: &str, line: u32, message: &str);
        fn debug(function_name: &str, file_name: &str, line: u32, message: &str);
        fn trace(function_name: &str, file_name: &str, line: u32, message: &str);
    }
}

fn error(function_name: &str, file_name: &str, line: u32, message: &str) {
    tracing::error!(function_name, file_name, line, message);
}

fn warn(function_name: &str, file_name: &str, line: u32, message: &str) {
    tracing::warn!(function_name, file_name, line, message);
}

fn info(function_name: &str, file_name: &str, line: u32, message: &str) {
    tracing::info!(function_name, file_name, line, message);
}

fn debug(function_name: &str, file_name: &str, line: u32, message: &str) {
    tracing::debug!(function_name, file_name, line, message);
}

fn trace(function_name: &str, file_name: &str, line: u32, message: &str) {
    tracing::trace!(function_name, file_name, line, message);
}
