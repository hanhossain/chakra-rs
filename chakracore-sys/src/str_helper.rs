pub use ffi::OptionalStr;
use widestring::U16CString;
use widestring::error::ContainsNul;

#[cxx::bridge(namespace = "chakra_rs::str_helper")]
mod ffi {
    extern "Rust" {
        fn to_lowercase(s: &String) -> String;
        fn to_raw_u16_str(s: &str) -> Result<*mut u16>;
        unsafe fn free_raw_str(p: *mut u16);
    }

    #[namespace = "chakra_rs"]
    #[derive(Debug, Clone, Default)]
    struct OptionalStr<'a> {
        has_value: bool,
        value: &'a str,
    }
}

impl<'a> From<OptionalStr<'a>> for Option<&'a str> {
    fn from(value: OptionalStr<'a>) -> Self {
        if value.has_value {
            Some(value.value)
        } else {
            None
        }
    }
}

impl<'a> From<Option<&'a str>> for OptionalStr<'a> {
    fn from(value: Option<&'a str>) -> Self {
        value
            .map(|x| Self {
                has_value: true,
                value: x,
            })
            .unwrap_or_default()
    }
}

fn to_lowercase(s: &String) -> String {
    s.to_lowercase()
}

#[tracing::instrument(level = "trace")]
fn to_raw_u16_str(s: &str) -> Result<*mut u16, ContainsNul<u16>> {
    Ok(U16CString::from_str(s)?.into_raw())
}

unsafe fn free_raw_str(p: *mut u16) {
    unsafe {
        let _ = U16CString::from_raw(p);
    }
}
