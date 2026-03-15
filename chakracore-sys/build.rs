fn main() {
    let out_dir = std::env::var("OUT_DIR").unwrap();
    let is_rust_analyzer = out_dir.contains("target/rust-analyzer");

    let mut cxx_bridge = cxx_build::bridge("src/lib.rs");
    cxx_bridge.include("../chakracore-cxx/bin/ch");

    if !is_rust_analyzer {
        cxx_bridge.compile("binding");
        let debug: bool = std::env::var("DEBUG").unwrap().parse::<bool>().unwrap();
        let optimized = cfg!(feature = "optimized-tests");
        let build_type = match (optimized, debug) {
            (false, true) => "Debug",
            (true, true) => "RelWithDebInfo",
            (_, false) => "Release",
        };

        let mut cc_config = cc::Build::new();
        if !cc_config.get_compiler().is_like_clang() {
            cc_config.compiler("clang");
        }

        let mut config = cmake::Config::new("..");
        config
            .init_c_cfg(cc_config)
            .generator("Ninja")
            .define("CMAKE_CXX_COMPILER", "clang++")
            .define("CMAKE_C_COMPILER", "clang")
            .profile(build_type);

        if cfg!(target_os = "macos") {
            config
                .define("DISABLE_JIT", "ON")
                .define("ICU_INCLUDE_PATH", "/opt/homebrew/opt/icu4c@77/include");
        }

        config.always_configure(true);
        let dst = config.build();

        println!("cargo::rustc-link-search=native={}/lib", dst.display());
        println!("cargo::rustc-link-lib=chhelper");
        println!("cargo::rustc-link-lib=ChakraCoreStatic");

        if cfg!(target_os = "macos") {
            println!("cargo::rustc-link-search=native=/opt/homebrew/opt/icu4c@77/lib");
            println!("cargo::rustc-link-lib=static=icudata");

            println!("cargo::rustc-link-lib=framework=CoreFoundation");
        }

        println!("cargo::rustc-link-lib=icuuc");
        println!("cargo::rustc-link-lib=icui18n");
    }

    println!("cargo::rerun-if-changed=../chakracore-cxx/");
    println!("cargo::rerun-if-changed=../CMakeLists.txt");
}
