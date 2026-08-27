#[cxx::bridge(namespace = "chakra_rs::fs")]
mod ffi {
    extern "Rust" {
        fn read_binary_file(path: &str) -> Result<Vec<u8>>;
        fn file_to_string(path: &str) -> Result<String>;
    }
}

#[tracing::instrument(err)]
fn read_binary_file(path: &str) -> std::io::Result<Vec<u8>> {
    tracing::trace!("reading file");
    std::fs::read(path)
}

#[tracing::instrument(err)]
fn file_to_string(path: &str) -> std::io::Result<String> {
    let bytes = std::fs::read(path)?;
    Ok(String::from_utf8_lossy(&bytes).into())
}
