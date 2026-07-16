//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Js
{
    class WebAssemblyModule;
}

namespace Wasm
{
    class WasmFunctionInfo;
    struct WasmReaderInfo
    {
        typename WriteBarrierFieldTypeTraits<WasmFunctionInfo *>::Type m_funcInfo;
        typename WriteBarrierFieldTypeTraits<Js::WebAssemblyModule *>::Type m_module;
    };
} // namespace Wasm
