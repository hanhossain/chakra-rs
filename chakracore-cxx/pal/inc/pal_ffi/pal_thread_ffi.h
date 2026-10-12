#pragma once
#include <rust/cxx.h>
#include "pal_mstypes.h"

namespace pal_ffi {
    HANDLE CreateThread(rust::Fn<uint32_t(void *, HANDLE)> func, void *param, uint32_t creationFlags);
}