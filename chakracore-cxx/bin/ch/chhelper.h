#pragma once
#include <cstdint>
#include "rust/cxx.h"

using c_char16_t = char16_t;

int main_internal(int argc, char** c_argv, uint32_t snapInterval, uint32_t snapHistoryLength, uint32_t startEventCount,
    const bool doTTRecord, const bool doTTReplay, rust::String ttUri);

const char16_t *get_hello_world();
