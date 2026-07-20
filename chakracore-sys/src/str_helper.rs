#[cxx::bridge(namespace = "chakra_rs::str_helper")]
mod ffi {
    extern "Rust" {
        fn to_lowercase(s: &String) -> String;

        fn split_args(args: &mut Vec<String>) -> Vec<String>;
    }
}

fn to_lowercase(s: &String) -> String {
    s.to_lowercase()
}

fn split_args(args: &mut Vec<String>) -> Vec<String> {
    let Some(start) = args
        .iter()
        .find(|x| *x == "-args")
        .and_then(|x| args.element_offset(x))
    else {
        return Vec::new();
    };

    let Some(end) = args
        .iter()
        .find(|x| *x == "-endargs")
        .and_then(|x| args.element_offset(x))
    else {
        return Vec::new();
    };

    let mut config_flags = args.drain(start..=end).collect::<Vec<_>>();

    // remove "-endargs"
    config_flags.remove(config_flags.len() - 1);

    // remove "-args"
    config_flags.remove(0);
    config_flags
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_split_args_with_args() {
        let mut args = ["a", "b", "-args", "c", "d", "-endargs"]
            .into_iter()
            .map(|x| x.to_owned())
            .collect::<Vec<_>>();
        let split = split_args(&mut args);
        assert_eq!(args, vec!["a", "b"]);
        assert_eq!(split, vec!["c", "d"]);
    }

    #[test]
    fn test_split_args_with_empty_args() {
        let mut args = ["a", "b", "-args", "-endargs"]
            .into_iter()
            .map(|x| x.to_owned())
            .collect::<Vec<_>>();
        let split = split_args(&mut args);
        assert_eq!(args, vec!["a", "b"]);
        assert_eq!(split, Vec::<String>::new());
    }

    #[test]
    fn test_split_args_with_no_args() {
        let mut args = ["a", "b"]
            .into_iter()
            .map(|x| x.to_owned())
            .collect::<Vec<_>>();
        let split = split_args(&mut args);
        assert_eq!(args, vec!["a", "b"]);
        assert_eq!(split, Vec::<String>::new());
    }
}
