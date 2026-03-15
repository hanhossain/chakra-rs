fn main() {
    let out_dir = std::env::var("OUT_DIR").unwrap();
    let is_rust_analyzer = out_dir.contains("target/rust-analyzer");
    let config = cxx_build::bridge("src/lib.rs");

    if !is_rust_analyzer {
        config.compile("chakra-binding");
    }
}
