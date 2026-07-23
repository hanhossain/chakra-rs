use chakracore_sys::config::CoreConfig;
use std::process::ExitCode;

fn main() -> ExitCode {
    let args: Vec<_> = std::env::args().collect();
    let Some(chakra_args) = ChakraArgs::new(args) else {
        chakracore_sys::chhelper::print_usage();
        return ExitCode::FAILURE;
    };

    if chakra_args.version {
        print_version();
        return ExitCode::SUCCESS;
    }

    if chakra_args.help {
        chakracore_sys::chhelper::print_usage();
        return ExitCode::SUCCESS;
    }

    chakracore::run(chakra_args.config)
}

fn print_version() {
    println!(
        "{} version {}",
        env!("CARGO_PKG_NAME"),
        env!("CARGO_PKG_VERSION")
    );
}

#[derive(Debug, Clone, Default)]
struct ChakraArgs {
    version: bool,
    help: bool,
    config: CoreConfig,
}

impl ChakraArgs {
    fn new(args: Vec<String>) -> Option<Self> {
        if args.len() < 2 {
            return None;
        }

        for arg in args.iter() {
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
        }

        let mut core_config: CoreConfig = serde_json::from_str(&args[1]).unwrap();
        // TODO (hanhossain): remove this insert
        // insert exe name since c++ code expects it still
        core_config.args.insert(0, args[0].clone());
        Some(ChakraArgs {
            config: core_config,
            ..Default::default()
        })
    }
}
