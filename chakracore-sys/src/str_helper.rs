#[cxx::bridge(namespace = "chakra_rs::str_helper")]
mod ffi {
    extern "Rust" {
        fn to_lowercase(s: &String) -> String;
    }
}

fn to_lowercase(s: &String) -> String {
    s.to_lowercase()
}
