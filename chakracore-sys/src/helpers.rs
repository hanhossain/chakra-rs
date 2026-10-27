#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("Core/CmdParser.h");

        type CmdLineArgsParser;
        #[Self = "CmdLineArgsParser"]
        fn New() -> UniquePtr<CmdLineArgsParser>;
        fn Parse(self: Pin<&mut CmdLineArgsParser>, vargs: &Vec<String>) -> i32;
    }
}

pub fn set_config_flags(vargs: &Vec<String>) -> Result<(), Error> {
    let mut parser = ffi::CmdLineArgsParser::New();
    let res = parser.pin_mut().Parse(vargs);
    if res == 0 {
        Ok(())
    } else {
        Err(Error::ParseConfig(res))
    }
}

#[derive(thiserror::Error, Debug)]
pub enum Error {
    #[error("Failed to parse config. Hresult: {0}")]
    ParseConfig(i32),
}
