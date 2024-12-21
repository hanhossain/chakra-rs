fn main() {
    let mut cc_config = cc::Build::new();
    if !cc_config.get_compiler().is_like_clang() {
        cc_config.compiler("clang");
    }

    cmake::Config::new("chakracore-cxx")
        .init_c_cfg(cc_config)
        .generator("Ninja")
        .define("DISABLE_JIT", "ON")
        .define("CMAKE_CXX_COMPILER", "clang++")
        .define("CMAKE_C_COMPILER", "clang")
        .build_target("ch")
        .build();
}
