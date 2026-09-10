#pragma once
#include "ChakraCommon.h"
#include "rust/cxx.h"
int32_t CreateParserStateAndRunScript(rust::Str fileName, const rust::String &contents, const rust::String &fullPath,
                                      JsRuntimeHandle &chRuntime, JsRuntimeAttributes jsrtAttributes);
int32_t CreateAndRunSerializedScript(rust::Str fileName, const rust::String &contents, const rust::String &fullPath,
                                     JsRuntimeHandle &chRuntime, JsRuntimeAttributes jsrtAttributes);
int32_t ExecuteTest(const rust::String &filename, const rust::String &fileContents);
