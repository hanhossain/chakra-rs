#pragma once
#include <memory>
#include <string>
#include "rust/cxx.h"

int32_t ExecuteTest(const rust::String &filename, const std::shared_ptr<std::string>& fileContents);