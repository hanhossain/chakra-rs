#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("HostConfigFlags.h");

        type HostConfigFlags;

        #[namespace = "chakra_rs"]
        type ConfigContext = crate::config::ConfigContext;
        #[Self = "HostConfigFlags"]
        fn SetHostArgs(args: &Vec<String>, config: &ConfigContext);

        #[Self = "HostConfigFlags"]
        fn GetConfig() -> &'static ConfigContext;
    }
}
