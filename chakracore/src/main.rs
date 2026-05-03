use std::process::ExitCode;

fn main() -> ExitCode {
    let args: Vec<_> = std::env::args().collect();
    let (exit_code, _) = chakracore::main_internal(&args);
    exit_code
}
