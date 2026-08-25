use std::path::PathBuf;

fn main() {
    let bridges = [
        "src/chhelper.rs",
        "src/config.rs",
        "src/filesystem.rs",
        "src/host_config.rs",
        "src/logger.rs",
        "src/rt_interface.rs",
        "src/str_helper.rs",
    ];
    let mut cxx_bridge = cxx_build::bridges(bridges);

    if !cxx_bridge.get_compiler().is_like_clang() {
        cxx_bridge.compiler("clang");
    }

    let c_compiler = cxx_bridge.clone();

    let icu_include_path = "/opt/homebrew/opt/icu4c/include";

    cxx_bridge
        .std("c++23")
        .flag("-fcolor-diagnostics")
        // disable c++ warnings until regular cmake build no longer has warnings
        .warnings(false)
        .flag("-fms-extensions")
        .define("PAL_STDCPP_COMPAT", None)
        .include("../chakracore-cxx/bin/ch")
        .include("../chakracore-cxx/bin/ChakraCore")
        .include("../chakracore-cxx/ffi/include")
        .include("../chakracore-cxx/lib/Common")
        .include("../chakracore-cxx/lib/Common/Interface/include")
        .include("../chakracore-cxx/lib/Common/PlatformAgnostic/include")
        .include("../chakracore-cxx/lib/Jsrt")
        .include("../chakracore-cxx/lib/Runtime")
        .include("../chakracore-cxx/pal/inc");

    if cfg!(target_os = "macos") {
        cxx_bridge.include(icu_include_path);
    }
    cxx_bridge.compile("binding");

    let out_dir = std::env::var("OUT_DIR").unwrap();
    let target = std::env::var("TARGET").unwrap();
    if !out_dir.contains(&target) {
        let cxx_header = format!("{out_dir}/cxxbridge/include/rust/cxx.h");
        let docker_cxx_header = PathBuf::from("../target/docker/rust/cxx.h");
        dbg!(&cxx_header, &docker_cxx_header);
        std::fs::create_dir_all(docker_cxx_header.parent().unwrap()).unwrap();
        std::fs::copy(cxx_header, docker_cxx_header).unwrap();
    }

    if cfg!(feature = "compile-cpp") {
        if let Ok(chakra_build) = std::env::var("CHAKRA_BUILD") {
            update_local_link_search(&chakra_build, "chakracore-cxx/bin/ch");
            println!("cargo::rerun-if-changed={chakra_build}/chakracore-cxx/bin/ch");

            update_local_link_search(&chakra_build, "chakracore-cxx/lib");
            println!("cargo::rerun-if-changed={chakra_build}/chakracore-cxx/lib");

            update_local_link_search(&chakra_build, "chakracore-cxx/ffi");
            println!("cargo::rerun-if-changed={chakra_build}/chakracore-cxx/ffi");
        } else {
            let debug: bool = std::env::var("DEBUG").unwrap().parse::<bool>().unwrap();
            let optimized = cfg!(feature = "optimized-tests");
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
                .define("CMAKE_CXX_COMPILER", "clang++")
                .define("CMAKE_C_COMPILER", "clang")
                .define("CMAKE_COLOR_DIAGNOSTICS", "ON")
                .profile(build_type)
                .build_target("chhelper");

            if cfg!(target_os = "macos") {
                config
                    .define("DISABLE_JIT", "ON")
                    .define("ICU_INCLUDE_PATH", icu_include_path);
            }

            config.always_configure(true);
            let mut dst = config.build();
            dst.push("build");
            let chakra_build = dst.to_str().unwrap();
            update_local_link_search(chakra_build, "chakracore-cxx/bin/ch");
            update_local_link_search(chakra_build, "chakracore-cxx/lib");
            update_local_link_search(chakra_build, "chakracore-cxx/ffi");
        }

        println!("cargo::rustc-link-lib=chhelper");
        println!("cargo::rustc-link-lib=ChakraCoreStatic");
        println!("cargo::rustc-link-lib=Chakra.Ffi");

        if cfg!(target_os = "macos") {
            println!("cargo::rustc-link-search=native=/opt/homebrew/opt/icu4c/lib");

            println!("cargo::rustc-link-lib=framework=CoreFoundation");
        }

        println!("cargo::rustc-link-lib=icuuc");
        println!("cargo::rustc-link-lib=icui18n");
    }

    println!("cargo::rerun-if-changed=../chakracore-cxx/bin/");
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
