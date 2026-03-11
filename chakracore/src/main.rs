#[cxx::bridge]
mod ffi {
    #[namespace = "simple"]
    unsafe extern "C++" {
        include!("simple.h");

        type Simple;
        fn hello_world(&self);
        fn Simple_new() -> UniquePtr<Simple>;
    }
}

fn main() {
    println!("hello world");
    let s = ffi::Simple_new();
    s.hello_world();
}
