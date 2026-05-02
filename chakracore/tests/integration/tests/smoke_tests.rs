use std::path::PathBuf;
use std::process::ExitCode;

#[test]
fn hello() {
    let source = PathBuf::from(env!("CARGO_MANIFEST_DIR"))
        .join("../chakracore-cxx/test/Basics/hello.js")
        .to_string_lossy()
        .to_string();

    let args = ["characore".to_owned(), source];
    let exit_code = chakracore::main_internal(&args);

    let actual = chakra::CONSOLE_LOGS.lock().unwrap().clone();
    let expected = vec!["hello world", "PASS"];
    assert_eq!(actual, expected);

    assert_eq!(exit_code, ExitCode::SUCCESS);
}
