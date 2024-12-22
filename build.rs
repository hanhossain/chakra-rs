use std::path::PathBuf;

fn main() {
    let target = std::env::var("TARGET").unwrap();
    if target.contains("windows") {
        build_msvc();
    } else {
        build_cmake();
    }
}

fn build_cmake() {
    let mut cc_config = cc::Build::new();
    if !cc_config.get_compiler().is_like_clang() {
        cc_config.compiler("clang");
    }

    cmake::Config::new("chakracore-cxx")
        .init_c_cfg(cc_config)
        .generator("Ninja")
        .define("DISABLE_JIT", "ON")
        .define("CMAKE_CXX_COMPILER", "clang++")
        .define("CMAKE_C_COMPILER", "clang")
        .build_target("ch")
        .build();
}

fn build_msvc() {
    let build = PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("chakracore-cxx/test/ci.buildone.cmd");
    let mut build = std::process::Command::new(build);
    let status = build.args(["x64", "test"]).status().unwrap();
    if !status.success() {
        panic!("Failed to build ChakraCore. Status: {status}");
    }
}
