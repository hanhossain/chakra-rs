#[cxx::bridge]
pub mod ffi {
    #[namespace = "chakra_rs::chhelper"]
    extern "Rust" {
        fn print_usage();
    }

    unsafe extern "C++" {
        include!("chhelper.h");
        include!("chakracore-sys/src/config.rs.h");

        #[namespace = "chakra_rs::config"]
        type CoreConfig = crate::config::CoreConfig;

        fn main_internal(config: CoreConfig) -> i32;
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
