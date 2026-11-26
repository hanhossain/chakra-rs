use std::path::{Path, PathBuf};

fn main() {
    let bridges = [
        "src/common/util.rs",
        "src/config.rs",
        "src/helpers.rs",
        "src/jsrt/ffi.rs",
        "src/logger.rs",
        "src/pal.rs",
        "src/runtime/platform_agnostic.rs",
        "src/str_helper.rs",
    ];
    let mut cxx_bridge = cxx_build::bridges(bridges);

    if !cxx_bridge.get_compiler().is_like_clang() {
        println!("cargo::error=Must use a clang compiler");
        return;
    }

    let c_compiler = cxx_bridge.clone();

    let icu_path = Path::new("/opt/homebrew/opt/icu4c@78");

    let target_os = std::env::var("CARGO_CFG_TARGET_OS").unwrap();
    let target_arch = std::env::var("CARGO_CFG_TARGET_ARCH").unwrap();

    println!("cargo::rustc-check-cfg=cfg(disable_jit)");
    println!("cargo::rustc-check-cfg=cfg(enable_native_codegen)");
    if target_arch == "x86_64" {
        println!("cargo::rustc-cfg=enable_native_codegen");
    } else {
        println!("cargo::rustc-cfg=disable_jit");
    }

    cxx_bridge
        .std("c++23")
        .flag("-fcolor-diagnostics")
        // disable c++ warnings until regular cmake build no longer has warnings
        .warnings(false)
        .flag("-fms-extensions")
        .define("PAL_STDCPP_COMPAT", None)
        .include("../chakracore-cxx/ffi/include")
        .include("../chakracore-cxx/lib/Common")
        .include("../chakracore-cxx/lib/Common/Interface/include")
        .include("../chakracore-cxx/lib/Common/PlatformAgnostic/include")
        .include("../chakracore-cxx/lib/Jsrt")
        .include("../chakracore-cxx/lib/Runtime")
        .include("../chakracore-cxx/pal/inc");

    if target_os == "macos" {
        cxx_bridge.include(icu_path.join("include"));
    }

    if target_arch == "x86_64" {
        cxx_bridge.define("ENABLE_NATIVE_CODEGEN", Some("1"));
    } else {
        cxx_bridge.define("ENABLE_NATIVE_CODEGEN", Some("0"));
        cxx_bridge.define("DISABLE_JIT", Some("1"));
    }

    cxx_bridge.compile("binding");

    let out_dir = std::env::var("OUT_DIR").unwrap();
    let target = std::env::var("TARGET").unwrap();
    if !out_dir.contains(&target) && std::env::var("RUSTC_WRAPPER").is_err() {
        let cxx_header = format!("{out_dir}/cxxbridge/include/rust/cxx.h");
        let docker_cxx_header = PathBuf::from("../target/docker/rust/cxx.h");
        dbg!(&cxx_header, &docker_cxx_header);
        let cxx_header_content = std::fs::read(&cxx_header).unwrap();
        let docker_header_content = std::fs::read(&docker_cxx_header)
            .inspect_err(|err| eprintln!("Failed to read file: {err}"))
            .unwrap_or_default();
        if cxx_header_content != docker_header_content {
            std::fs::create_dir_all(docker_cxx_header.parent().unwrap()).unwrap();
            std::fs::copy(cxx_header, docker_cxx_header).unwrap();
        }
    }

    if std::env::var("CARGO_FEATURE_COMPILE_CPP").is_ok() {
        if let Ok(chakra_build) = std::env::var("CHAKRA_BUILD") {
            update_local_link_search(&chakra_build, "chakracore-cxx/lib");
            println!("cargo::rerun-if-changed={chakra_build}/chakracore-cxx/lib");

            update_local_link_search(&chakra_build, "chakracore-cxx/ffi");
            println!("cargo::rerun-if-changed={chakra_build}/chakracore-cxx/ffi");
        } else {
            let debug: bool = std::env::var("DEBUG").unwrap().parse::<bool>().unwrap();
            let optimized = std::env::var("CARGO_FEATURE_OPTIMIZED_TESTS").is_ok();
            let build_type = match (optimized, debug) {
                (false, true) => "Debug",
                (true, true) => "RelWithDebInfo",
                (_, false) => "Release",
            };

            let mut config = cmake::Config::new("..");
            config
                .init_c_cfg(c_compiler.clone())
                .init_cxx_cfg(c_compiler)
                .generator("Ninja")
                .define("CMAKE_COLOR_DIAGNOSTICS", "ON")
                .env("CLICOLOR_FORCE", "1")
                .profile(build_type)
                .build_target("ChakraCoreStatic");

            if target_os == "macos" {
                config
                    .define("DISABLE_JIT", "ON")
                    .define("ICU_INCLUDE_PATH", icu_path.join("include"));
            }

            config.always_configure(false);
            let mut dst = config.build();
            dst.push("build");
            let chakra_build = dst.to_str().unwrap();
            update_local_link_search(chakra_build, "chakracore-cxx/lib");
            update_local_link_search(chakra_build, "chakracore-cxx/ffi");
        }

        println!("cargo::rustc-link-lib=ChakraCoreStatic");
        println!("cargo::rustc-link-lib=Chakra.Ffi");

        if target_os == "macos" {
            println!("cargo::rustc-link-search=native={}/lib", icu_path.display());

            println!("cargo::rustc-link-lib=framework=CoreFoundation");
        }

        println!("cargo::rustc-link-lib=icuuc");
        println!("cargo::rustc-link-lib=icui18n");
    }

    println!("cargo::rerun-if-changed=../chakracore-cxx/ffi/");
    println!("cargo::rerun-if-changed=../chakracore-cxx/lib/");
    println!("cargo::rerun-if-changed=../chakracore-cxx/pal/");
    println!("cargo::rerun-if-changed=../chakracore-cxx/CMakeLists.txt");
    println!("cargo::rerun-if-changed=../CMakeLists.txt");
    println!("cargo::rerun-if-env-changed=CHAKRA_BUILD");
}

fn update_local_link_search(chakra_build: &str, path: &str) {
    println!("cargo::rustc-link-search=native={chakra_build}/{path}");
}
