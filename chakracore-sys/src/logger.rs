use tracing::span::EnteredSpan;

#[cxx::bridge(namespace = "chakra_rs::log")]
mod ffi {
    extern "Rust" {
        fn error(function_name: &str, file_name: &str, line: u32, message: &str);
        fn warn(function_name: &str, file_name: &str, line: u32, message: &str);
        fn info(function_name: &str, file_name: &str, line: u32, message: &str);
        fn debug(function_name: &str, file_name: &str, line: u32, message: &str);
        fn trace(function_name: &str, file_name: &str, line: u32, message: &str);

        type ForeignSpan;
        #[Self = "ForeignSpan"]
        fn create(function_name: &str) -> Box<ForeignSpan>;
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

struct ForeignSpan {
    _span: EnteredSpan,
}

impl ForeignSpan {
    fn create(function_name: &str) -> Box<ForeignSpan> {
        let span = tracing::info_span!("c++", name = function_name).entered();
        Box::new(ForeignSpan { _span: span })
    }
}
