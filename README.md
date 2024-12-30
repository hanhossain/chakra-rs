# Migrates chakracore to rust

## Testing
Configure cmake
```sh
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DICU_INCLUDE_PATH=/opt/homebrew/opt/icu4c@74/include -DDISABLE_JIT=ON -GNinja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang ..
```

Build and test
```sh
ninja
ninja check
```

## Dependency Graph
Generate graph
```sh
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DICU_INCLUDE_PATH=/opt/homebrew/opt/icu4c@74/include -DDISABLE_JIT=ON -GNinja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang --graphviz=graph.dot ..
```

Visualize graph
```sh
brew install graphviz
dot -Tsvg -o graph.svg graph.dot
open graph.svg
```

## Migrate tests
### Header
```rust
mod common;
const DIRECTORY: &str = "chakracore-cxx/test/{}";
```

### add todo
```rust
// <test>
```

```rust

// TODO (hanhossain): migrate
// <test>
```

### files only
```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*</default>\n.*</test>
```

```rust
#[test]
fn $1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        ..Default::default()
    };
    common::run_test(&test);
}
```

### files and baseline
```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<baseline>(.*)</baseline>\n.*</default>\n.*</test>
```

```rust
#[test]
fn $1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        baseline_path: Some("$2"),
        ..Default::default()
    };
    common::run_test(&test);
}
```

### files and compile flags
```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<compile-flags>(.*)</compile-flags>\n.*</default>\n.*</test>
```

```rust
#[test]
fn $1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        compile_flags: vec![todo!("$2")],
        ..Default::default()
    };
    common::run_test(&test);
}
```

### files, baseline, and compile flags
```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<baseline>(.*)</baseline>\n.*<compile-flags>(.*)</compile-flags>\n.*</default>\n.*</test>
```

```rust
#[test]
fn $1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        baseline_path: Some("$2"),
        compile_flags: vec![todo!("$3")],
        ..Default::default()
    };
    common::run_test(&test);
}
```

### files, compile flags, and baseline
```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*<compile-flags>(.*)</compile-flags>\n.*<baseline>(.*)</baseline>\n.*</default>\n.*</test>
```

```rust
#[test]
fn $1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "$1.js",
        baseline_path: Some("$3"),
        compile_flags: vec![todo!("$2")],
        ..Default::default()
    };
    common::run_test(&test);
}
```

### replace compile flags todos with spaces (until done)
```re
todo!\("(\S+) 
```

```rust
"$1",todo!("
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
