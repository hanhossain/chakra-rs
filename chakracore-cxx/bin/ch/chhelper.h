#pragma once
#include "ChakraCommon.h"
#include "rust/cxx.h"
int32_t RunScript(rust::Str fileName, const rust::String &contents, JsValueRef bufferValue,
                  const rust::String &fullPath, JsValueRef parserStateCache);
