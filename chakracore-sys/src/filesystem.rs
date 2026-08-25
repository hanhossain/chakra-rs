#[cxx::bridge(namespace = "chakra_rs::fs")]
mod ffi {
    extern "Rust" {
        fn read_binary_file(s: &str) -> Result<Vec<u8>>;
    }
}

fn read_binary_file(s: &str) -> std::io::Result<Vec<u8>> {
    std::fs::read(s)
}
