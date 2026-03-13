fn main() {
    // set rstest timeout to 60s
    println!("cargo::rustc-env=RSTEST_TIMEOUT=60");

    println!("cargo::rustc-check-cfg=cfg(disable_jit)");
    if !cfg!(target_arch = "x86_64") && cfg!(unix) {
        println!("cargo::rustc-cfg=disable_jit");
    }

    if cfg!(target_os = "macos") {
        println!("cargo::rustc-link-search=native=/opt/homebrew/opt/icu4c@77/lib");
        println!("cargo::rustc-link-lib=icudata");

        println!("cargo::rustc-link-lib=framework=CoreFoundation");
    }

    println!("cargo::rustc-link-lib=icuuc");
    println!("cargo::rustc-link-lib=icui18n");
}
