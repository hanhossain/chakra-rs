use std::path::PathBuf;

fn main() {
    let optimized_test = cfg!(feature = "optimized-test");

    let target = std::env::var("TARGET").unwrap();
    if target.contains("windows") {
        build_msvc(optimized_test);
    } else {
        #[cfg(unix)]
        build_cmake(optimized_test);
    }

    // set rstest timeout to 60s
    println!("cargo::rustc-env=RSTEST_TIMEOUT=60");

    println!("cargo::rustc-check-cfg=cfg(disable_jit)");
    if !cfg!(target_arch = "x86_64") && cfg!(unix) {
        println!("cargo::rustc-cfg=disable_jit");
    }
}

#[cfg(unix)]
fn build_cmake(optimized_test: bool) {
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

    if optimized_test {
        config.profile("RelWithDebInfo");
    }

    let target = std::env::var("TARGET").unwrap();
    if target.contains("darwin") {
        config
            .define("DISABLE_JIT", "ON")
            .define("ICU_INCLUDE_PATH", "/opt/homebrew/opt/icu4c@74/include");
    }

    config.build();
}

fn build_msvc(optimized_test: bool) {
    let debug: bool = std::env::var("DEBUG").unwrap().parse::<bool>().unwrap();
    let configuration = match (optimized_test, debug) {
        (true, _) => "Test",
        (false, true) => "Debug",
        (false, false) => "Release",
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
