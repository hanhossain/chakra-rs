use std::env::current_exe;

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
    pub fn new(args: &mut Vec<String>) -> Option<Self> {
        if args.len() < 2 {
            return None;
        }

        let mut chakra_args = ChakraArgs::default();
        let mut tt_indices = Vec::new();

        for (index, arg) in args.iter().enumerate() {
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
                tt_indices.push(index);
            } else if arg.starts_with("-TTReplay=") {
                chakra_args.do_tt_replay = true;
                let record_uri = arg.split('=').next().unwrap_or_default().to_owned();
                chakra_args.tt_uri = get_ttd_directory(record_uri);
                tt_indices.push(index);
            } else if arg.starts_with("-TTSnapInterval=") {
                let raw_value = arg.split('=').next().unwrap_or_default();
                chakra_args.tt_snap_interval = raw_value.parse::<u32>().unwrap_or(u32::MAX);
                tt_indices.push(index);
            } else if arg.starts_with("-TTHistoryLength=") {
                let raw_value = arg.split('=').next().unwrap_or_default();
                chakra_args.tt_snap_history_length = raw_value.parse::<u32>().unwrap_or(u32::MAX);
                tt_indices.push(index);
            } else if arg.starts_with("-TTDStartEvent=") {
                let raw_value = arg.split('=').next().unwrap_or_default();
                chakra_args.ttd_start_event_count = raw_value.parse::<u32>().unwrap_or(1);
                tt_indices.push(index);
            }
        }

        for index in tt_indices.into_iter().rev() {
            args.remove(index);
        }

        Some(chakra_args)
    }
}
