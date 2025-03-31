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

## TODO

Preprocessor directives to remove:
- ios
  - `__IOS__`
- android
  - `__ANDROID__`
- any arch that's not x64 or arm64
  - `_M_IX86`
  - `_ARM32`
  - `_M_ARM`
  - `TARGET_32`

Files to remove:
- any files with // TODO (hanhossain): remove file

typedefs/defines to remove:
- __int64
  - int64
  - uint64
- __int32
  - int32
  - uint32
- __int16
  - int16
  - uint16
