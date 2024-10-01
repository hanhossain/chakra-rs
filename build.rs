fn main() {
    let dst = cmake::Config::new("chakracore-cxx")
        .generator("Ninja")
        .define("ICU_INCLUDE_PATH", "/opt/homebrew/opt/icu4c/include")
        .define("DISABLE_JIT", "ON")
        .build_target("all")
        .build();
    println!("cargo:rustc-link-search=native={}/build", dst.display());
    println!("cargo:rustc-link-lib=ChakraCore");
}
