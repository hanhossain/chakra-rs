use std::path::PathBuf;

fn main() {
    let target = std::env::var("TARGET").unwrap();
    if target.contains("windows") {
        build_msvc();
    } else {
        #[cfg(unix)]
        build_cmake();
    }
}

#[cfg(unix)]
fn build_cmake() {
    let mut cc_config = cc::Build::new();
    if !cc_config.get_compiler().is_like_clang() {
        cc_config.compiler("clang");
    }

    let mut config = cmake::Config::new("chakracore-cxx");
    config
        .init_c_cfg(cc_config)
        .generator("Ninja")
        .define("CMAKE_CXX_COMPILER", "clang++")
        .define("CMAKE_C_COMPILER", "clang")
        .build_target("ch");

    let target = std::env::var("TARGET").unwrap();
    if target.contains("darwin") {
        config.define("DISABLE_JIT", "ON");
    }

    config.build();
}

fn build_msvc() {
    let mut msbuild = std::process::Command::new("msbuild");
    let sln = PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("chakracore-cxx/Build/Chakra.Core.sln");
    let out_dir = PathBuf::from(std::env::var("OUT_DIR").unwrap()).join("build/");
    let status = msbuild
        .arg("/p:Configuration=Test")
        .arg("/p:Platform=x64")
        .arg(format!("/p:OutDir={}", out_dir.display()))
        .arg("/m")
        .arg(sln)
        .status()
        .unwrap();

    if !status.success() {
        panic!("Failed to build ChakraCore. Status: {status}");
    }
}
