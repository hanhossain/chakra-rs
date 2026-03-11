fn main() {
    // // only compile c++ when asked to, this allows cargo check to skip it until we need it.
    // if cfg!(feature = "compile-cpp") {
    //     let debug: bool = std::env::var("DEBUG").unwrap().parse::<bool>().unwrap();
    //     let optimized = cfg!(feature = "optimized-tests");
    //     build_cmake(optimized, debug);
    // }

    // // set rstest timeout to 60s
    // println!("cargo::rustc-env=RSTEST_TIMEOUT=60");

    // println!("cargo::rustc-check-cfg=cfg(disable_jit)");
    // if !cfg!(target_arch = "x86_64") && cfg!(unix) {
    //     println!("cargo::rustc-cfg=disable_jit");
    // }
    cxx_build::bridge("src/main.rs")
        .include("../simple-cxx")
        .compile("binding");

    let dst = cmake::Config::new("..")
        .generator("Ninja")
        .build_target("simple-cxx")
        .build();
    println!(
        "cargo::rustc-link-search=native={}/build/simple-cxx",
        dst.display()
    );
    println!("cargo::rustc-link-lib=simple-cxx");

    println!("cargo::rerun-if-changed=../simple-cxx/");
    println!("cargo::rerun-if-changed=../CMakeLists.txt");
}

// fn build_cmake(optimized: bool, debug: bool) {
//     let mut cc_config = cc::Build::new();
//     if !cc_config.get_compiler().is_like_clang() {
//         cc_config.compiler("clang");
//     }

//     let build_type = match (optimized, debug) {
//         (false, true) => "Debug",
//         (true, true) => "RelWithDebInfo",
//         (_, false) => "Release",
//     };

//     let mut config = cmake::Config::new("..");
//     config
//         .init_c_cfg(cc_config)
//         .generator("Ninja")
//         .define("CMAKE_CXX_COMPILER", "clang++")
//         .define("CMAKE_C_COMPILER", "clang")
//         .profile(build_type)
//         .build_target("ch");

//     let target = std::env::var("TARGET").unwrap();
//     if target.contains("darwin") {
//         config
//             .define("DISABLE_JIT", "ON")
//             .define("ICU_INCLUDE_PATH", "/opt/homebrew/opt/icu4c@77/include");
//     }

//     config.always_configure(false);
//     config.build();

//     println!("cargo::rerun-if-changed=../chakracore-cxx/");
// }
