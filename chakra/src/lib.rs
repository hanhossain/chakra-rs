use std::env::current_exe;
use std::sync::Mutex;

#[cxx::bridge(namespace = "chakra")]
mod ffi {
    extern "Rust" {
        fn print_usage();
        fn print_std_out(s: String);
    }
}

pub static CONSOLE_LOGS: Mutex<Vec<String>> = Mutex::new(Vec::new());

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

fn print_std_out(s: String) {
    CONSOLE_LOGS.lock().unwrap().push(s);
}

fn get_ttd_directory(record_uri: String) -> String {
    let exe = current_exe().unwrap();
    let exe_dir = exe.parent().unwrap().to_owned();

    let directory = exe_dir.join("_ttdlog").join(record_uri);
    std::fs::create_dir_all(&directory).unwrap();
    directory.to_string_lossy().into_owned()
}

#[derive(Debug, Clone, Default)]
pub struct ChakraArgs {
    pub version: bool,
    pub help: bool,
    pub tt_snap_interval: u32,
    pub tt_snap_history_length: u32,
    pub ttd_start_event_count: u32,
    pub do_tt_record: bool,
    pub do_tt_replay: bool,
    pub tt_uri: String,
}

impl ChakraArgs {
    pub fn new(args: &[String]) -> Option<Self> {
        if args.len() < 2 {
            return None;
        }

        let mut chakra_args = ChakraArgs::default();

        for arg in args {
            if arg == "-v" || arg == "--version" {
                return Some(ChakraArgs {
                    version: true,
                    ..ChakraArgs::default()
                });
            }

            if arg == "-h" || arg == "--help" {
                return Some(ChakraArgs {
                    help: true,
                    ..ChakraArgs::default()
                });
            }

            if arg.starts_with("-TTRecord=") {
                chakra_args.do_tt_record = true;
                let record_uri = arg.split('=').next().unwrap_or_default().to_owned();
                chakra_args.tt_uri = get_ttd_directory(record_uri);
            } else if arg.starts_with("-TTReplay=") {
                chakra_args.do_tt_replay = true;
                let record_uri = arg.split('=').next().unwrap_or_default().to_owned();
                chakra_args.tt_uri = get_ttd_directory(record_uri);
            } else if arg.starts_with("-TTSnapInterval=") {
                let raw_value = arg.split('=').next().unwrap_or_default();
                chakra_args.tt_snap_interval = raw_value.parse::<u32>().unwrap_or(u32::MAX);
            } else if arg.starts_with("-TTHistoryLength=") {
                let raw_value = arg.split('=').next().unwrap_or_default();
                chakra_args.tt_snap_history_length = raw_value.parse::<u32>().unwrap_or(u32::MAX);
            } else if arg.starts_with("-TTDStartEvent=") {
                let raw_value = arg.split('=').next().unwrap_or_default();
                chakra_args.ttd_start_event_count = raw_value.parse::<u32>().unwrap_or(1);
            }
        }

        Some(chakra_args)
    }
}
