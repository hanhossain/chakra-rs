#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("HostConfigFlags.h");

        type HostConfigFlags;

        #[Self = "HostConfigFlags"]
        fn PrintUsageString();

        #[Self = "HostConfigFlags"]
        fn SetHostArgs(args: &Vec<String>);
    }
}
