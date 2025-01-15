use std::path::PathBuf;

fn main() {
    let opt_level = std::env::var("OPT_LEVEL").unwrap();

    // only compile c++ when asked to, this allows cargo check to skip it until we need it.
    if cfg!(feature = "compile-cpp") {
        let debug: bool = std::env::var("DEBUG").unwrap().parse::<bool>().unwrap();

        let target = std::env::var("TARGET").unwrap();
        if target.contains("windows") {
            build_msvc(&opt_level, debug);
        } else {
            #[cfg(unix)]
            build_cmake();
        }
    }

    println!("cargo::rustc-check-cfg=cfg(optimized)");
    if opt_level != "0" {
        println!("cargo::rustc-cfg=optimized");
    }

    // set rstest timeout to 60s
    println!("cargo::rustc-env=RSTEST_TIMEOUT=60");

    println!("cargo::rustc-check-cfg=cfg(disable_jit)");
    if !cfg!(target_arch = "x86_64") && cfg!(unix) {
        println!("cargo::rustc-cfg=disable_jit");
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
        config
            .define("DISABLE_JIT", "ON")
            .define("ICU_INCLUDE_PATH", "/opt/homebrew/opt/icu4c/include");
    }

    config.build();
}

fn build_msvc(opt_level: &str, debug: bool) {
    let configuration = match (opt_level, debug) {
        ("0", _) => "Debug",
        (_, true) => "Test",
        (_, false) => "Release",
    };

    let mut msbuild = std::process::Command::new("msbuild");
    let sln = PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("chakracore-cxx/Build/Chakra.Core.sln");
    let out_dir = PathBuf::from(std::env::var("OUT_DIR").unwrap()).join("build/");

    let status = msbuild
        .arg(format!("/p:Configuration={configuration}"))
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
