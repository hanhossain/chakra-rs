#[cxx::bridge(namespace = "chakra")]
mod ffi {
    extern "Rust" {
        fn print_usage();
    }
}

fn print_usage() {
    println!("\nUsage: ch [-v|-version] [-h|-help] [-?] [flaglist] <source file>");
    println!("\t-v|-version\t\tDisplays version info");
    println!("\t-h|-help\t\tDisplays this help message");
    println!("\t-?\t\t\tDisplays this help message with complete [flaglist] info");
}
