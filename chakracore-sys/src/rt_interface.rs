#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("stdafx.h");
        include!("ChakraRtInterface.h");

        type ChakraRTInterface;

        #[Self = "ChakraRTInterface"]
        fn InitializeTestHooks(vargs: &Vec<String>) -> i32;
    }
}
