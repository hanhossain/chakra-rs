#pragma once
#include "ChakraCommon.h"
#include "rust/cxx.h"
int32_t RunScript(rust::Str fileName, const rust::String &contents, JsValueRef bufferValue,
                  const rust::String &fullPath, JsValueRef parserStateCache);
int32_t CreateParserStateAndRunScript(rust::Str fileName, const rust::String &contents, const rust::String &fullPath,
                                      JsRuntimeHandle &chRuntime, JsRuntimeAttributes jsrtAttributes);
int32_t CreateAndRunSerializedScript(rust::Str fileName, const rust::String &contents, const rust::String &fullPath,
                                     JsRuntimeHandle &chRuntime, JsRuntimeAttributes jsrtAttributes, JsValueRef bufferVal);
