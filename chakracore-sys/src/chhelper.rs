#[cxx::bridge]
pub mod ffi {
    #[namespace = "chakra_rs::chhelper"]
    extern "Rust" {
        fn print_usage();
    }

    extern "C++" {
        include!("chhelper.h");

        unsafe fn main_internal(
            argc: i32,
            argv: *mut *mut c_char,
            snap_interval: u32,
            snap_history_length: u32,
            start_event_count: u32,
            do_tt_record: bool,
            do_tt_replay: bool,
            tt_uri: String,
        ) -> i32;
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
