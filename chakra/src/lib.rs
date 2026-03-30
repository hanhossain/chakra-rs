#[cxx::bridge(namespace = "chakra")]
mod ffi {
    extern "Rust" {
        fn print_usage();
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

#[derive(Debug, Clone, Copy, Default)]
pub struct ChakraArgs {
    pub version: bool,
    pub help: bool,
}

impl ChakraArgs {
    pub fn new(args: Vec<String>) -> Option<Self> {
        if args.len() < 2 {
            return None;
        }

        for arg in &args {
            if arg == "-v" || arg == "--version" {
                return Some(ChakraArgs {
                    version: true,
                    ..ChakraArgs::default()
                });
            }

            if arg == "-h" || arg == "--help" || (cfg!(debug_assertions) && arg == "-?") {
                return Some(ChakraArgs {
                    help: true,
                    ..ChakraArgs::default()
                });
            }
        }

        Some(ChakraArgs::default())
    }
}
