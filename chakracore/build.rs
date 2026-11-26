fn main() {
    // set rstest timeout to 60s
    println!("cargo::rustc-env=RSTEST_TIMEOUT=60");

    println!("cargo::rustc-check-cfg=cfg(disable_jit)");

    let target_arch = std::env::var("CARGO_CFG_TARGET_ARCH").unwrap();
    if target_arch != "x86_64" {
        println!("cargo::rustc-cfg=disable_jit");
    }
}
