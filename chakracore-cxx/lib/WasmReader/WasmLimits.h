//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

#ifdef ENABLE_WASM
namespace Wasm {
    class Limits {
        // The limits are agreed upon with other engines for consistency.
        static const uint32_t MaxTypes = 1000000;
        static const uint32_t MaxFunctions = 1000000;
        static const uint32_t MaxImports = 100000;
        static const uint32_t MaxExports = 100000;
        static const uint32_t MaxGlobals = 1000000;
        static const uint32_t MaxDataSegments = 100000;
        static const uint32_t MaxElementSegments = 10000000;
        static const uint32_t MaxTableSize = DEFAULT_CONFIG_WasmMaxTableSize;

        static const uint32_t MaxStringSize = 100000;
        static const uint32_t MaxFunctionLocals = 50000;
        static const uint32_t MaxFunctionParams = 1000;
        static const uint32_t MaxFunctionReturns = 10000; // todo::We need to figure out what is the right limit here
        static const uint32_t MaxBrTableElems = 1000000;

        static const uint32_t MaxMemoryInitialPages = Js::ArrayBuffer::MaxArrayBufferLength / Js::WebAssembly::PageSize;
        static const uint32_t MaxMemoryMaximumPages = 65536;
        static const uint32_t MaxModuleSize = 1024 * 1024 * 1024;
        static const uint32_t MaxFunctionSize = 7654321;
    public:
        // Use accessors to easily switch to config flags if needed
        static uint32_t GetMaxTypes() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxTypes; }
        static uint32_t GetMaxFunctions() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxFunctions; }
        static uint32_t GetMaxImports() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxImports; }
        static uint32_t GetMaxExports() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxExports; }
        static uint32_t GetMaxGlobals() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxGlobals; }
        static uint32_t GetMaxDataSegments() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxDataSegments; }
        static uint32_t GetMaxElementSegments() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxElementSegments; }
        static uint32_t GetMaxTableSize() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : CONFIG_FLAG(WasmMaxTableSize); }
        static uint32_t GetMaxStringSize() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxStringSize; }
        static uint32_t GetMaxFunctionLocals() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxFunctionLocals; }
        static uint32_t GetMaxFunctionParams() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxFunctionParams; }
        static uint32_t GetMaxFunctionReturns() {
            if (CONFIG_FLAG(WasmMultiValue))
            {
                return CONFIG_FLAG(WasmIgnoreLimits) ? (UINT32_MAX - 1) : MaxFunctionReturns;
            }
            return 1;
        }
        static unsigned long GetMaxBrTableElems() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxBrTableElems; }
        static uint32_t GetMaxMemoryInitialPages() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxMemoryInitialPages; }
        static uint32_t GetMaxMemoryMaximumPages() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxMemoryMaximumPages; }
        static uint32_t GetMaxModuleSize() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxModuleSize; }
        static uint32_t GetMaxFunctionSize() { return CONFIG_FLAG(WasmIgnoreLimits) ? UINT32_MAX : MaxFunctionSize; }
    };
}
#endif
