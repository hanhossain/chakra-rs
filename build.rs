fn main() {
    cmake::Config::new("chakracore-cxx")
        .generator("Ninja")
        .define("DISABLE_JIT", "ON")
        .define("CMAKE_CXX_COMPILER", "clang++")
        .define("CMAKE_C_COMPILER", "clang")
        .build_target("ch")
        .build();
}
