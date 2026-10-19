pub use ffi::TestHooks;

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("TestHooks.h");
        type TestHooks;

        #[Self = "TestHooks"]
        fn SetConfigFlags(vargs: &Vec<String>) -> i32;

        #[Self = "TestHooks"]
        fn SetAssertToConsoleFlag(flag: bool) -> i32;
    }
}
