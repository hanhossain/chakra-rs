#pragma once
#include <cstdint>
#include "rust/cxx.h"

int main_internal(int argc, char** c_argv, uint32_t snapInterval, uint32_t snapHistoryLength, uint32_t startEventCount,
    const bool doTTRecord, const bool doTTReplay, rust::String ttUri);
