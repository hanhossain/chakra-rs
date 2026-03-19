fn main() {
    cxx_build::CFG.exported_header_links.push("chakra");

    let out_dir = std::env::var("OUT_DIR").unwrap();
    let is_rust_analyzer = out_dir.contains("target/rust-analyzer");

    let mut bridge_config = cxx_build::bridge("src/lib.rs");
    
    if !bridge_config.get_compiler().is_like_clang() {
        bridge_config.compiler("clang");
    }
    
    bridge_config.include("../chakracore-cxx/bin/ch");

    let mut bridge_config_cpp = bridge_config.clone();
    bridge_config_cpp.std("c++23");

    if !is_rust_analyzer {
        bridge_config_cpp.compile("binding");

        let debug: bool = std::env::var("DEBUG").unwrap().parse::<bool>().unwrap();
        let optimized = cfg!(feature = "optimized-tests");
        let build_type = match (optimized, debug) {
            (false, true) => "Debug",
            (true, true) => "RelWithDebInfo",
            (_, false) => "Release",
        };

        let mut config = cmake::Config::new("..");
        config
            .init_c_cfg(bridge_config)
            .init_cxx_cfg(bridge_config_cpp)
            .generator("Ninja")
            .define("CMAKE_CXX_COMPILER", "clang++")
            .define("CMAKE_C_COMPILER", "clang")
            .profile(build_type)
            .build_target("chhelper");

        if cfg!(target_os = "macos") {
            config
                .define("DISABLE_JIT", "ON")
                .define("ICU_INCLUDE_PATH", "/opt/homebrew/opt/icu4c@77/include");
        }

        config.always_configure(false);
        let dst = config.build();

        println!("cargo::rustc-link-search=native={}/lib", dst.display());
        println!("cargo::rustc-link-lib=chhelper");
        println!("cargo::rustc-link-lib=ChakraCoreStatic");

        if cfg!(target_os = "macos") {
            println!("cargo::rustc-link-search=native=/opt/homebrew/opt/icu4c@77/lib");
            println!("cargo::rustc-link-lib=icudata");

            println!("cargo::rustc-link-lib=framework=CoreFoundation");
        }

        println!("cargo::rustc-link-lib=icuuc");
        println!("cargo::rustc-link-lib=icui18n");
    }

    println!("cargo::rerun-if-changed=../chakracore-cxx/");
    println!("cargo::rerun-if-changed=../CMakeLists.txt");
}
