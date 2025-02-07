# Migrates chakracore to rust

## Testing

Configure cmake

```sh
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DICU_INCLUDE_PATH=/opt/homebrew/opt/icu4c/include -DDISABLE_JIT=ON -GNinja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang ..
```

Build and test

```sh
ninja
ninja check
```

## Dependency Graph

Generate graph

```sh
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DICU_INCLUDE_PATH=/opt/homebrew/opt/icu4c/include -DDISABLE_JIT=ON -GNinja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang --graphviz=graph.dot ..
```

See [dependency-graph.md](./dependency-graph.md)

Visualize graph

```sh
brew install graphviz
dot -Tsvg -o graph.svg graph.dot
open graph.svg
```

## Migrate tests

### Header

```rust
use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const DIRECTORY: &str = "chakracore-cxx/test/{}";
```

### add todo

```re
//\s*<test>
```

```rust

// TODO (hanhossain): migrate
$0
```

### files only

```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*</default>\n.*</test>
```

```rust
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn $1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
```

### files and baseline

```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<baseline>(.*)</baseline>\n.*</default>\n.*</test>
```

```rust
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn $1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        baseline_path: Some("$2"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
```

### files and compile flags

```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<compile-flags>(.*)</compile-flags>\n.*</default>\n.*</test>
```

```rust
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn $1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        compile_flags: vec![todo!("$2")],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
```

### files, compile flags, and tags

```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<compile-flags>(.*)</compile-flags>\n.*<tags>(.*)</tags>\n.*</default>\n.*</test>
```

```rust
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn $1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        compile_flags: vec![todo!("$2")],
        tags: HashSet::from([todo!("$3")]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
```

### files, baseline, and compile flags

```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<baseline>(.*)</baseline>\n.*<compile-flags>(.*)</compile-flags>\n.*</default>\n.*</test>
```

```rust
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn $1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        baseline_path: Some("$2"),
        compile_flags: vec![todo!("$3")],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
```

### files, baseline, compile flags, and tags

```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<baseline>(.*)</baseline>\n.*<compile-flags>(.*)</compile-flags>\n.*<tags>(.*)</tags>\n.*</default>\n.*</test>
```

```rust
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn $1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        baseline_path: Some("$2"),
        compile_flags: vec![todo!("$3")],
        tags: HashSet::from([todo!("$4")]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
```

### files, compile flags, and baseline

```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<compile-flags>(.*)</compile-flags>\n.*<baseline>(.*)</baseline>\n.*</default>\n.*</test>
```

```rust
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn $1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        baseline_path: Some("$3"),
        compile_flags: vec![todo!("$2")],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
```

### replace tags todos (until done)

```re
todo!\(\n?\s*"(\w+),?
```

```rust
"$1", todo!("
```

### replace compile flags todos with spaces (until done)

```re
todo!\(\n?\s*"(\S+) 
```

```rust
"$1", todo!("
```

### replace compile flags todos with no spaces

```re
todo!\(("\S+")\)
```

```rust
$1
```

### replace dashes and periods in function name

```re
^(fn.*)(-|\.)
```

```rust
$1_
```

### remove empty todo

```re
,\s*todo!\("\s*"\)
```

```rust
```
