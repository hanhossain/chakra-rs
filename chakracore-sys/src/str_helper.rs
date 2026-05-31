#[cxx::bridge(namespace = "chakracore_sys::str_helper")]
mod ffi {
    extern "Rust" {
        fn to_lowercase(s: &String) -> String;
    }
}

fn to_lowercase(s: &String) -> String {
    s.to_lowercase()
}
