mod array;
#[cfg(all(not(disable_jit), feature = "optimized-tests"))]
mod asm_js;
#[cfg(all(not(disable_jit), feature = "optimized-tests"))]
mod asm_js_float;
#[cfg(all(not(disable_jit), feature = "optimized-tests"))]
mod asm_js_parser;
#[cfg(not(disable_jit))]
mod bailout;
mod basics;
#[cfg(feature = "optimized-tests")]
mod bigint;
#[cfg(feature = "optimized-tests")]
mod boolean;
#[cfg(feature = "optimized-tests")]
mod bugs;
mod closures;
#[cfg(windows)]
mod config_parsing;
#[cfg(feature = "optimized-tests")]
mod control_flow;
#[cfg(feature = "optimized-tests")]
mod conversions;
mod date;
#[cfg(feature = "optimized-tests")]
mod debugger;
#[cfg(feature = "optimized-tests")]
mod debugger_common;
#[cfg(not(feature = "optimized-tests"))]
mod dynamic_code;
#[cfg(feature = "optimized-tests")]
mod eh;
#[cfg(feature = "optimized-tests")]
mod error;
mod es5;
mod es6;
#[cfg(not(disable_jit))]
mod es6_generator_jit;
#[cfg(feature = "optimized-tests")]
mod es6module;
#[cfg(feature = "optimized-tests")]
mod es7;
#[cfg(feature = "optimized-tests")]
mod fieldopts;
#[cfg(all(not(disable_jit), feature = "optimized-tests"))]
mod fixed_fields;
#[cfg(all(not(disable_jit), feature = "optimized-tests"))]
mod flow_graph;
mod function;
#[cfg(feature = "optimized-tests")]
mod generated;
mod global_functions;
#[cfg(feature = "optimized-tests")]
mod inline_caches;
#[cfg(all(not(disable_jit), feature = "optimized-tests"))]
mod inlining;
#[cfg(feature = "optimized-tests")]
mod intl;
mod json;
#[cfg(feature = "optimized-tests")]
mod let_const;
#[cfg(feature = "optimized-tests")]
mod lib;
#[cfg(feature = "optimized-tests")]
mod math;
mod miscellaneous;
#[cfg(feature = "optimized-tests")]
mod number;
#[cfg(feature = "optimized-tests")]
mod object;
#[cfg(feature = "optimized-tests")]
mod operators;
mod optimizer;
#[cfg(all(not(disable_jit), feature = "optimized-tests"))]
mod perf_hint;
#[cfg(all(not(disable_jit), feature = "optimized-tests"))]
mod pre;
#[cfg(feature = "optimized-tests")]
mod prototypes;
#[cfg(feature = "optimized-tests")]
mod reg_alloc;
mod regex;
#[cfg(feature = "optimized-tests")]
mod rwc;
#[cfg(feature = "optimized-tests")]
mod scanner;
#[cfg(feature = "compile-cpp")]
mod smoke_tests;
#[cfg(feature = "optimized-tests")]
mod stack_trace;
mod stackfunc;
mod strict;
#[cfg(feature = "optimized-tests")]
mod strings;
#[cfg(feature = "optimized-tests")]
mod switch_statement;
#[cfg(all(target_arch = "x86_64", feature = "optimized-tests"))]
mod tagged_floats;
mod tagged_integers;
#[cfg(feature = "optimized-tests")]
mod test262;
#[cfg(feature = "optimized-tests")]
mod test_loop;
#[cfg(feature = "optimized-tests")]
mod try_catch;
#[cfg(feature = "optimized-tests")]
mod ttbasic;
#[cfg(feature = "optimized-tests")]
mod ttexecute_basic;
mod typed_array;
#[cfg(feature = "optimized-tests")]
mod unified_regex;
#[cfg(feature = "optimized-tests")]
mod unit_test_framework;
#[cfg(feature = "optimized-tests")]
mod utf8;
#[cfg(feature = "optimized-tests")]
mod wasm;
#[cfg(all(not(disable_jit), windows))]
mod wasm_simd;
#[cfg(feature = "optimized-tests")]
mod wasm_spec;
#[cfg(feature = "optimized-tests")]
mod wasm_spec_multi_value;
