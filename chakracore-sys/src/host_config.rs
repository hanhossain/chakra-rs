#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("HostConfigFlags.h");

        type HostConfigFlags;

        #[Self = "HostConfigFlags"]
        fn PrintUsageString();

        #[namespace = "chakra_rs::config"]
        type CoreConfig = crate::config::CoreConfig;
        #[Self = "HostConfigFlags"]
        fn SetHostArgs(args: &Vec<String>, core_config: &CoreConfig);

        #[Self = "HostConfigFlags"]
        fn GetCoreConfig() -> &'static CoreConfig;
    }
}
