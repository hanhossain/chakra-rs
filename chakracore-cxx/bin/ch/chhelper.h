#pragma once
#include "ChakraCommon.h"
#include "rust/cxx.h"

int32_t ExecuteTest(JsRuntimeHandle &runtime, const rust::String &filename, const rust::String &fileContents);