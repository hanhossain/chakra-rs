fn main() {
    cmake::Config::new("chakracore-cxx")
        .generator("Ninja")
        .define("DISABLE_JIT", "ON")
        .build_target("ch")
        .build();
}
