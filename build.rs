fn main() {
    //let mut cpp_config = cc::Build::new();
    //if !cpp_config.get_compiler().is_like_clang() {
    //    cpp_config.compiler("clang++");
    //}

    let mut c_config = cc::Build::new();
    if !c_config.get_compiler().is_like_clang() {
        c_config.compiler("clang");
    }

    let dst = cmake::Config::new("chakracore-cxx")
        .init_c_cfg(c_config)
        //.init_cxx_cfg(cpp_config)
        .generator("Ninja")
        //.define("ICU_INCLUDE_PATH", "/opt/homebrew/opt/icu4c/include")
        .define("DISABLE_JIT", "ON")
        .define("CMAKE_CXX_COMPILER", "clang++")
        .define("CMAKE_C_COMPILER", "clang")
        //.build_target("all")
        .build_target("ChakraCore")
        .build();
    println!("cargo:rustc-link-search=native={}/build", dst.display());
    println!("cargo:rustc-link-lib=ChakraCore");
}
