mod array;
#[cfg(not(disable_jit))]
mod asm_js;
#[cfg(not(disable_jit))]
mod asm_js_float;
#[cfg(not(disable_jit))]
mod asm_js_parser;
#[cfg(not(disable_jit))]
mod bailout;
mod basics;
mod bigint;
mod boolean;
mod bugs;
mod closures;
#[cfg(windows)]
mod config_parsing;
mod control_flow;
mod conversions;
mod date;
mod debugger;
mod debugger_common;
#[cfg(not(feature = "optimized-tests"))]
mod dynamic_code;
mod eh;
mod error;
mod es5;
mod es6;
#[cfg(not(disable_jit))]
mod es6_generator_jit;
mod es6module;
mod es7;
mod fieldopts;
#[cfg(not(disable_jit))]
mod fixed_fields;
#[cfg(not(disable_jit))]
mod flow_graph;
mod function;
mod generated;
mod global_functions;
mod inline_caches;
#[cfg(not(disable_jit))]
mod inlining;
mod intl;
mod json;
mod let_const;
mod lib;
mod math;
mod miscellaneous;
mod number;
mod object;
mod operators;
mod optimizer;
#[cfg(not(disable_jit))]
mod perf_hint;
#[cfg(not(disable_jit))]
mod pre;
mod prototypes;
mod reg_alloc;
mod regex;
mod rwc;
mod scanner;
mod smoke_tests;
mod stack_trace;
// TODO (hanhossain): migrate
// mod stackfunc;
mod strict;
mod strings;
mod switch_statement;
#[cfg(target_arch = "x86_64")]
mod tagged_floats;
mod tagged_integers;
mod test262;
mod test_loop;
mod try_catch;
// TODO (hanhossain): migrate
// mod ttbasic;
// TODO (hanhossain): migrate
// mod ttexecute_basic;
mod typed_array;
mod unified_regex;
mod unit_test_framework;
mod utf8;
// TODO (hanhossain): migrate
// mod wasm;
// TODO (hanhossain): migrate
// mod wasm_simd;
// TODO (hanhossain): migrate
// mod wasm_spec;
// TODO (hanhossain): migrate
// mod wasm_spec_multi_value;
