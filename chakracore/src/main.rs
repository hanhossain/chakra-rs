use chakracore_sys::chhelper::ffi::Abstractions;
use chakracore_sys::config::CoreConfig;
use tracing_subscriber::filter::LevelFilter;
use tracing_subscriber::fmt::format::FmtSpan;

fn setup_tracing() {
    tracing_subscriber::fmt()
        .with_max_level(LevelFilter::TRACE)
        .with_writer(std::io::stderr)
        .with_span_events(FmtSpan::NEW | FmtSpan::CLOSE)
        .init();
}

fn main() -> anyhow::Result<()> {
    setup_tracing();

    let wait_for_debugger = std::env::var("WAIT_FOR_DEBUGGER")
        .unwrap_or_default()
        .parse::<bool>()
        .unwrap_or_default();

    if wait_for_debugger {
        let mut counter = 0;
        while !Abstractions::IsDebuggerPresent() && counter < 30 {
            let skip_assert = true;
            tracing::debug!(counter, skip_assert, "waiting for debugger");
            std::thread::sleep(std::time::Duration::from_secs(1));
            counter += 1;
        }

        if counter >= 30 {
            anyhow::bail!("debugger did not attach in time");
        } else {
            tracing::info!("debugger attached");
        }
    } else {
        tracing::debug!("not waiting for debugger");
    }

    let args: Vec<_> = std::env::args().collect();
    let Some(chakra_args) = ChakraArgs::new(args) else {
        chakracore_sys::chhelper::print_usage();
        anyhow::bail!("invalid arguments");
    };

    if chakra_args.version {
        print_version();
        return Ok(());
    }

    if chakra_args.help {
        chakracore_sys::chhelper::print_usage();
        return Ok(());
    }

    chakracore::run(chakra_args.config)?;
    Ok(())
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
