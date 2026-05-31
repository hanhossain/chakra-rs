use std::path::PathBuf;

fn main() {
    let bridges = ["src/chhelper.rs", "src/str_helper.rs"];
    let mut cxx_bridge = cxx_build::bridges(bridges);
    cxx_bridge
        .include("../chakracore-cxx/bin/ch")
        .compile("binding");

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
            println!(
                "cargo::rustc-link-search=native={}/chakracore-cxx/bin/ch",
                chakra_build
            );
            println!(
                "cargo::rustc-link-search=native={}/chakracore-cxx/lib",
                chakra_build
            );
            println!(
                "cargo::rerun-if-changed={}/chakracore-cxx/bin/ch",
                chakra_build
            );
            println!(
                "cargo::rerun-if-changed={}/chakracore-cxx/lib",
                chakra_build
            );
        } else {
            let debug: bool = std::env::var("DEBUG").unwrap().parse::<bool>().unwrap();
            let optimized = cfg!(feature = "optimized-tests");
            let build_type = match (optimized, debug) {
                (false, true) => "Debug",
                (true, true) => "RelWithDebInfo",
                (_, false) => "Release",
            };

            if !cxx_bridge.get_compiler().is_like_clang() {
                cxx_bridge.compiler("clang");
            }

            let mut config = cmake::Config::new("..");
            config
                .init_c_cfg(cxx_bridge.clone())
                .init_cxx_cfg(cxx_bridge)
                .generator("Ninja")
                .define("CMAKE_CXX_COMPILER", "clang++")
                .define("CMAKE_C_COMPILER", "clang")
                .profile(build_type);

            if cfg!(target_os = "macos") {
                config
                    .define("DISABLE_JIT", "ON")
                    .define("ICU_INCLUDE_PATH", "/opt/homebrew/opt/icu4c/include");
            }

            config.always_configure(true);
            let dst = config.build();
            println!("cargo::rustc-link-search=native={}/lib", dst.display());
        }

        println!("cargo::rustc-link-lib=chhelper");
        println!("cargo::rustc-link-lib=ChakraCoreStatic");

        if cfg!(target_os = "macos") {
            println!("cargo::rustc-link-search=native=/opt/homebrew/opt/icu4c/lib");

            println!("cargo::rustc-link-lib=framework=CoreFoundation");
        }

        println!("cargo::rustc-link-lib=icuuc");
        println!("cargo::rustc-link-lib=icui18n");
    }

    println!("cargo::rerun-if-changed=../chakracore-cxx/bin/");
    println!("cargo::rerun-if-changed=../chakracore-cxx/lib/");
    println!("cargo::rerun-if-changed=../chakracore-cxx/pal/");
    println!("cargo::rerun-if-changed=../chakracore-cxx/CMakeLists.txt");
    println!("cargo::rerun-if-changed=../CMakeLists.txt");
    println!("cargo::rerun-if-env-changed=CHAKRA_BUILD");
}
