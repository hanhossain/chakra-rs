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
        static uint32_t GetMaxTypes() { return MaxTypes; }
        static uint32_t GetMaxFunctions() { return MaxFunctions; }
        static uint32_t GetMaxImports() { return MaxImports; }
        static uint32_t GetMaxExports() { return MaxExports; }
        static uint32_t GetMaxGlobals() { return MaxGlobals; }
        static uint32_t GetMaxDataSegments() { return MaxDataSegments; }
        static uint32_t GetMaxElementSegments() { return MaxElementSegments; }
        static uint32_t GetMaxTableSize() { return CONFIG_FLAG(WasmMaxTableSize); }
        static uint32_t GetMaxStringSize() { return MaxStringSize; }
        static uint32_t GetMaxFunctionLocals() { return MaxFunctionLocals; }
        static uint32_t GetMaxFunctionParams() { return MaxFunctionParams; }
        static uint32_t GetMaxFunctionReturns() {
            if (CONFIG_FLAG(WasmMultiValue))
            {
                return MaxFunctionReturns;
            }
            return 1;
        }
        static unsigned long GetMaxBrTableElems() { return MaxBrTableElems; }
        static uint32_t GetMaxMemoryInitialPages() { return MaxMemoryInitialPages; }
        static uint32_t GetMaxMemoryMaximumPages() { return MaxMemoryMaximumPages; }
        static uint32_t GetMaxModuleSize() { return MaxModuleSize; }
        static uint32_t GetMaxFunctionSize() { return MaxFunctionSize; }
    };
}
#endif
