#pragma once
#include "ChakraCommon.h"
#include "MessageQueue.h"
#include "rust/cxx.h"
#include <memory>
int32_t RunScript(rust::Str fileName, const rust::String &contents, JsValueRef bufferValue,
                  const rust::String &fullPath, JsValueRef parserStateCache,
                  const std::unique_ptr<MessageQueue> &messageQueue);
