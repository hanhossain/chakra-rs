#[cxx::bridge(namespace = "chakra")]
mod ffi {
    extern "Rust" {
        fn print_usage();
        fn print_version(bin_name: &str, version: &str);
    }
}

fn print_usage() {
    #[cfg(debug_assertions)]
    {
        println!("\nUsage: ch [-v|-version] [-h|-help] [-?] [flaglist] <source file>");
        println!("\t-v|-version\t\tDisplays version info");
        println!("\t-h|-help\t\tDisplays this help message");
        println!("\t-?\t\t\tDisplays this help message with complete [flaglist] info");
    }

    #[cfg(not(debug_assertions))]
    {
        println!("\nUsage: ch [-v|-version] [-h|-help|-?] <source file>");
        println!(
            "Note: [flaglist] is not supported in Release builds; try a Debug or Test build to enable these flags."
        );
        println!("\t-v|-version\t\tDisplays version info");
        println!("\t-h|-help|-?\t\tDisplays this help message");
    }
}

fn print_version(bin_name: &str, version: &str) {
    println!("{bin_name} version {version}");
}
