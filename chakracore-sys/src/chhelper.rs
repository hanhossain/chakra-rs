#[cxx::bridge]
pub mod ffi {
    #[namespace = "chakra_rs::chhelper"]
    extern "Rust" {
        fn print_usage();
    }

    unsafe extern "C++" {
        include!("ChakraCommon.h");
        include!("MessageQueue.h");
        include!("Util/Abstractions.h");

        type Abstractions;
        #[Self = "Abstractions"]
        fn IsDebuggerPresent() -> bool;

        type JsRuntimeHandle = crate::rt_interface::JsRuntimeHandle;
        type JsRuntimeAttributes = crate::rt_interface::ffi::JsRuntimeAttributes;
        type JsValueRef = crate::rt_interface::JsValueRef;

        type MessageQueue;
        #[Self = "MessageQueue"]
        fn New() -> UniquePtr<MessageQueue>;

        fn RemoveAll(self: Pin<&mut MessageQueue>);
        fn IsEmpty(self: Pin<&mut MessageQueue>) -> bool;
        fn ProcessAll(self: Pin<&mut MessageQueue>, filename: &str) -> i32;
    }
}

pub fn print_usage() {
    #[cfg(debug_assertions)]
    {
        println!("\nUsage: ch [-v|--version] [-h|--help] [-?] [flaglist] <source file>");
        println!("\t-v|--version\t\tDisplays version info");
        println!("\t-h|--help\t\tDisplays this help message");
        println!("\t-?\t\t\tDisplays this help message with complete [flaglist] info");
    }

    #[cfg(not(debug_assertions))]
    {
        println!("\nUsage: ch [-v|--version] [-h|--help|-?] <source file>");
        println!(
            "Note: [flaglist] is not supported in Release builds; try a Debug or Test build to enable these flags."
        );
        println!("\t-v|--version\t\tDisplays version info");
        println!("\t-h|--help|-?\t\tDisplays this help message");
    }
}
