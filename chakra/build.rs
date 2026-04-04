use std::path::PathBuf;

fn main() {
    cxx_build::bridge("src/lib.rs").compile("chakra-binding");

    let out_dir = std::env::var("OUT_DIR").unwrap();
    let target = std::env::var("TARGET").unwrap();
    if !out_dir.contains(&target) {
        let cxx_header = format!("{out_dir}/cxxbridge/include/rust/cxx.h");
        let manifest_dir = std::env::var("CARGO_MANIFEST_DIR").unwrap();
        let docker_cxx_header =
            PathBuf::from(format!("{}/../target/docker/rust/cxx.h", manifest_dir));
        dbg!(&cxx_header, &docker_cxx_header);
        std::fs::create_dir_all(docker_cxx_header.parent().unwrap()).unwrap();
        std::fs::copy(cxx_header, docker_cxx_header).unwrap();
    }
}
