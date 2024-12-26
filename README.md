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
### files only
```re
// TODO.*\n.*<test>\n.*<default>\n.*<files>(.*)\.js</files>\n.*</default>\n.*</test>
```

```rust
#[test]
fn $1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/$1.js",
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
        source_path: "chakracore-cxx/test/EH/$1.js",
        baseline_path: Some("chakracore-cxx/test/EH/$2"),
        ..Default::default()
    };
    common::run_test(&test);
}
```
