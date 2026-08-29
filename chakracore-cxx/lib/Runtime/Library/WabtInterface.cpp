//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifdef ENABLE_WABT
#include "wabtapi.h"
#include "Codex/Utf8Helper.h"
#include "chakra/strings.h"
#include <rust/cxx.h>

namespace Js
{

struct Context
{
    ArenaAllocator* allocator;
    ScriptContext* scriptContext;
};

static PropertyId propertyMap[] = {
    Js::PropertyIds::as,
    Js::PropertyIds::action,
    Js::PropertyIds::args,
    Js::PropertyIds::buffer,
    Js::PropertyIds::commands,
    Js::PropertyIds::expected,
    Js::PropertyIds::field,
    Js::PropertyIds::line,
    Js::PropertyIds::name,
    Js::PropertyIds::module,
    Js::PropertyIds::text,
    Js::PropertyIds::type,
    Js::PropertyIds::value,
};

bool SetProperty(Js::Var obj, PropertyId id, Js::Var value, void* user_data)
{
    static_assert((sizeof(propertyMap)/sizeof(PropertyId)) == ChakraWabt::PropertyIds::COUNT);
    Context* ctx = (Context*)user_data;
    Assert(id < ChakraWabt::PropertyIds::COUNT);
    return !!JavascriptOperators::OP_SetProperty(obj, propertyMap[id], value, ctx->scriptContext);
}
Js::Var CreateObject(void* user_data)
{
    Context* ctx = (Context*)user_data;
    return JavascriptOperators::NewJavascriptObjectNoArg(ctx->scriptContext);
}
Js::Var CreateArray(void* user_data)
{
    Context* ctx = (Context*)user_data;
    return JavascriptOperators::NewJavascriptArrayNoArg(ctx->scriptContext);
}
void Push(Js::Var arr, Js::Var obj, void* user_data)
{
    Context* ctx = (Context*)user_data;
    JavascriptArray::Push(ctx->scriptContext, arr, obj);
}
Js::Var Int32ToVar(int32_t value, void* user_data)
{
    Context* ctx = (Context*)user_data;
    return JavascriptNumber::ToVar(value, ctx->scriptContext);
}
Js::Var Int64ToVar(long value, void* user_data)
{
    Context* ctx = (Context*)user_data;
    return JavascriptNumber::ToVar(value, ctx->scriptContext);
}
Js::Var StringToVar(const char* src, uint length, void* user_data)
{
    Context* ctx = (Context*)user_data;
    charcount_t bufSize = 0;
    const std::u16string str = chakra::to_u16string(rust::Str{src, length});
    bufSize = str.length();
    Assert(bufSize < UINT32_MAX);
    return JavascriptString::NewCopyBuffer(str.c_str(), bufSize, ctx->scriptContext);
}

Js::Var CreateBuffer(const uint8_t* buf, uint size, void* user_data)
{
    Context* ctx = (Context*)user_data;
    ArrayBuffer* arrayBuffer = ctx->scriptContext->GetLibrary()->CreateArrayBuffer(size);
    js_memcpy_s(arrayBuffer->GetBuffer(), arrayBuffer->GetByteLength(), buf, size);
    return arrayBuffer;
}

Js::Var WabtInterface::EntryConvertWast2Wasm(RecyclableObject* function, CallInfo callInfo, ...)
{
    ScriptContext* scriptContext = function->GetScriptContext();
    PROBE_STACK(function->GetScriptContext(), Constants::MinStackDefault);

    ARGUMENTS(args, callInfo);
    AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");

    Assert(!(callInfo.Flags & CallFlags_New));

    if (args.Info.Count < 2 || !VarIs<JavascriptString>(args[1]))
    {
        JavascriptError::ThrowTypeError(scriptContext, WASMERR_NeedBufferSource);
    }
    bool isSpecText = false;
    if (args.Info.Count > 2)
    {
        // optional config object
        if (!JavascriptOperators::IsObject(args[2]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_NeedObject, u"config");
        }
        DynamicObject * configObject = VarTo<DynamicObject>(args[2]);

        Js::Var isSpecVar = JavascriptOperators::OP_GetProperty(configObject, PropertyIds::spec, scriptContext);
        isSpecText = JavascriptConversion::ToBool(isSpecVar, scriptContext);
    }

    ArenaAllocator arena(u"Wast2Wasm", scriptContext->GetThreadContext()->GetPageAllocator(), Throw::OutOfMemory);
    Context context;
    size_t wastSize;
    char* wastBuffer = nullptr;

    ENTER_PINNED_SCOPE(JavascriptString, string);
    string = (JavascriptString*)args[1];
    const char16_t* str = string->GetString();
    context.allocator = &arena;
    context.scriptContext = scriptContext;

    rust::String s{str};
    wastBuffer = static_cast<char *>(std::malloc(s.length() + 1));
    strcpy(wastBuffer, s.c_str());
    wastSize = s.length();
    LEAVE_PINNED_SCOPE();   //  string

    try
    {
        ChakraWabt::SpecContext spec;
        ChakraWabt::ChakraContext wabtCtx;
        wabtCtx.user_data = &context;
        wabtCtx.createBuffer = CreateBuffer;
        wabtCtx.features.sign_extends = CONFIG_FLAG(WasmSignExtends);
        wabtCtx.features.threads = Wasm::Threads::IsEnabled();
        wabtCtx.features.simd = Wasm::Simd::IsEnabled();
        wabtCtx.features.sat_float_to_int = Wasm::WasmNontrapping::IsEnabled();
        if (isSpecText)
        {
            wabtCtx.spec = &spec;
            spec.setProperty = SetProperty;
            spec.int32ToVar = Int32ToVar;
            spec.int64ToVar = Int64ToVar;
            spec.stringToVar = StringToVar;
            spec.createObject = CreateObject;
            spec.createArray = CreateArray;
            spec.push = Push;
        }
        void* result = ChakraWabt::ConvertWast2Wasm(wabtCtx, wastBuffer, (uint)wastSize, isSpecText);
        if (result == nullptr)
        {
            return scriptContext->GetLibrary()->GetUndefined();
        }
        return result;
    }
    catch (ChakraWabt::WabtAPIError& e)
    {
        const auto s = chakra::to_u16string(e.message);
        JavascriptError::ThrowTypeErrorVar(scriptContext, WABTERR_WabtError, s.c_str());
    }
}
}
#endif // ENABLE_WABT
