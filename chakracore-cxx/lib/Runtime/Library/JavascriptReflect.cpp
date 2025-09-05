//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeLibraryPch.h"

namespace Js
{
    Var JavascriptReflect::EntryDefineProperty(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        Var undefinedValue = scriptContext->GetLibrary()->GetUndefined();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.defineProperty");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if  (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.defineProperty");
        }

        Var propertyKey, attributes;
        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.defineProperty");
        }
        Var target = args[1];

        propertyKey = args.Info.Count > 2 ? args[2] : undefinedValue;
        PropertyRecord const * propertyRecord;
        JavascriptConversion::ToPropertyKey(propertyKey, scriptContext, &propertyRecord, nullptr);

        attributes = args.Info.Count > 3 ? args[3] : undefinedValue;
        PropertyDescriptor propertyDescriptor;
        if (!JavascriptOperators::ToPropertyDescriptor(attributes, &propertyDescriptor, scriptContext))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_PropertyDescriptor_Invalid, scriptContext->GetPropertyName(propertyRecord->GetPropertyId())->GetBuffer());
        }

        // If the object is HostDispatch try to invoke the operation remotely
        BOOL defineResult;
        if (JavascriptOperators::GetTypeId(target) == TypeIds_HostDispatch)
        {
            defineResult = VarTo<RecyclableObject>(target)->InvokeBuiltInOperationRemotely(EntryDefineProperty, args, nullptr);
        }
        else
        {
            defineResult = JavascriptObject::DefineOwnPropertyHelper(VarTo<RecyclableObject>(target), propertyRecord->GetPropertyId(), propertyDescriptor, scriptContext, false);
        }

        return scriptContext->GetLibrary()->GetTrueOrFalse(defineResult);
    }

    Var JavascriptReflect::EntryDeleteProperty(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        Var undefinedValue = scriptContext->GetLibrary()->GetUndefined();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.deleteProperty");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if  (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.deleteProperty");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.deleteProperty");
        }
        Var target = args[1];
        Var propertyKey = args.Info.Count > 2 ? args[2] : undefinedValue;

        return JavascriptOperators::OP_DeleteElementI(target, propertyKey, scriptContext);
    }

    Var JavascriptReflect::EntryGet(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        Var undefinedValue = scriptContext->GetLibrary()->GetUndefined();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.get");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if  (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.get");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.get");
        }
        Var target = args[1];
        Var propertyKey = args.Info.Count > 2 ? args[2] : undefinedValue;

        Var receiver = args.Info.Count > 3 ? args[3] : target;

        return JavascriptOperators::GetElementIHelper(VarTo<RecyclableObject>(target), propertyKey, receiver, scriptContext);
    }

    Var JavascriptReflect::EntryGetOwnPropertyDescriptor(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        Var undefinedValue = scriptContext->GetLibrary()->GetUndefined();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.getOwnPropertyDescriptor");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if  (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.getOwnPropertyDescriptor");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.getOwnPropertyDescriptor");
        }
        Var target = args[1];
        Var propertyKey = args.Info.Count > 2 ? args[2] : undefinedValue;

        if (JavascriptOperators::GetTypeId(target) == TypeIds_HostDispatch)
        {
            Var result;
            if (VarTo<RecyclableObject>(target)->InvokeBuiltInOperationRemotely(EntryGetOwnPropertyDescriptor, args, &result))
            {
                return result;
            }
        }

        return JavascriptObject::GetOwnPropertyDescriptorHelper(VarTo<RecyclableObject>(target), propertyKey, scriptContext);
    }

    Var JavascriptReflect::EntryGetPrototypeOf(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.getPrototypeOf");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if  (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.getPrototypeOf");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.getPrototypeOf");
        }
        Var target = args[1];
        return JavascriptObject::GetPrototypeOf(VarTo<RecyclableObject>(target), scriptContext);
    }

    Var JavascriptReflect::EntryHas(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        Var undefinedValue = scriptContext->GetLibrary()->GetUndefined();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.has");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if  (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.has");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.has");
        }
        Var target = args[1];

        Var propertyKey = args.Info.Count > 2 ? args[2] : undefinedValue;
        BOOL result = JavascriptOperators::OP_HasItem(target, propertyKey, scriptContext);
        return scriptContext->GetLibrary()->GetTrueOrFalse(result);
    }

    Var JavascriptReflect::EntryIsExtensible(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.issExtensible");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if  (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.isExtensible");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.iesExtensible");
        }
        Var target = args[1];
        RecyclableObject *object = VarTo<RecyclableObject>(target);
        BOOL isExtensible = object->IsExtensible();

        GlobalObject* globalObject = object->GetLibrary()->GetGlobalObject();
        if (isExtensible && globalObject != object && globalObject && (globalObject->ToThis() == object))
        {
            isExtensible = globalObject->IsExtensible();
        }

        return scriptContext->GetLibrary()->GetTrueOrFalse(isExtensible);
    }

    Var JavascriptReflect::EntryOwnKeys(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.ownKeys");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if  (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.ownKeys");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.ownKeys");
        }
        Var target = args[1];

        return JavascriptOperators::GetOwnPropertyKeys(target, scriptContext);
    }

    Var JavascriptReflect::EntryPreventExtensions(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.preventExtensions");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.preventExtensions");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.preventExtensions");
        }
        Var target = args[1];

        RecyclableObject* targetObj = VarTo<RecyclableObject>(target);
        GlobalObject* globalObject = targetObj->GetLibrary()->GetGlobalObject();
        if (globalObject != targetObj && globalObject && (globalObject->ToThis() == targetObj))
        {
            globalObject->PreventExtensions();
        }

        return  scriptContext->GetLibrary()->GetTrueOrFalse(targetObj->PreventExtensions());
    }

    Var JavascriptReflect::EntrySet(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        Var undefinedValue = scriptContext->GetLibrary()->GetUndefined();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.set");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.set");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.set");
        }
        Var target = args[1];
        Var propertyKey = args.Info.Count > 2 ? args[2] : undefinedValue;
        Var value = args.Info.Count > 3 ? args[3] : undefinedValue;
        Var receiver = args.Info.Count > 4 ? args[4] : target;

        target = JavascriptOperators::ToObject(target, scriptContext);
        BOOL result = JavascriptOperators::SetElementIHelper(receiver, VarTo<RecyclableObject>(target), propertyKey, value, scriptContext, PropertyOperationFlags::PropertyOperation_None);
        return scriptContext->GetLibrary()->GetTrueOrFalse(result);
    }

    Var JavascriptReflect::EntrySetPrototypeOf(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.setPrototypeOf");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if  (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.setPrototypeOf");
        }

        if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, u"Reflect.setPrototypeOf");
        }
        Var target = args[1];
        target = JavascriptOperators::ToObject(target, scriptContext);

        if (args.Info.Count < 3 || !JavascriptOperators::IsObjectOrNull(args[2]))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NotObjectOrNull, u"Object.setPrototypeOf");
        }

        RecyclableObject* newPrototype = VarTo<RecyclableObject>(args[2]);
        BOOL changeResult = JavascriptObject::ChangePrototype(VarTo<RecyclableObject>(target), newPrototype, /*validate*/false, scriptContext);

        return scriptContext->GetLibrary()->GetTrueOrFalse(changeResult);
    }

    Var JavascriptReflect::EntryApply(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        Var undefinedValue = scriptContext->GetLibrary()->GetUndefined();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.apply");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.apply");
        }

        Var target = args.Info.Count > 1 ? args[1] : undefinedValue;
        if (!JavascriptConversion::IsCallable(target))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedFunction, u"Reflect.apply");
        }
        Var thisArgument = args.Info.Count > 2 ? args[2] : undefinedValue;
        Var argArray = args.Info.Count > 3 ? args[3] : undefinedValue;
        if (!(JavascriptOperators::IsObjectType(JavascriptOperators::GetTypeId(argArray))))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedArrayLike, u"Reflect.apply");
        }

        return JavascriptFunction::ApplyHelper(VarTo<RecyclableObject>(target), thisArgument, argArray, scriptContext);
    }

    static const int STACK_ARGS_ALLOCA_THRESHOLD = 8;
    Var JavascriptReflect::EntryConstruct(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        Var undefinedValue = scriptContext->GetLibrary()->GetUndefined();
        AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Reflect.construct");

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        if (args.Info.Flags & CallFlags_New)
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_ErrorOnNew, u"Reflect.construct");
        }

        Var target = args.Info.Count > 1 ? args[1] : undefinedValue;

        if (!JavascriptOperators::IsConstructor(target))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_NeedConstructor, u"target");
        }

        Var newTarget = nullptr;
        if (args.Info.Count > 3)
        {
            newTarget = args[3];
            if (!JavascriptOperators::IsConstructor(newTarget))
            {
                JavascriptError::ThrowTypeError(scriptContext, JSERR_NeedConstructor, u"newTarget");
            }
        }
        else
        {
            newTarget = target;
        }

        RecyclableObject* thisArg = VarTo<RecyclableObject>(undefinedValue);
        if (newTarget != nullptr)
        {
            thisArg = JavascriptOperators::CreateFromConstructor(VarTo<RecyclableObject>(newTarget), scriptContext);
        }

        Var argArray = args.Info.Count > 2 ? args[2] : undefinedValue;
        AnalysisAssert(argArray != nullptr);
        if (!(JavascriptOperators::IsObjectType(JavascriptOperators::GetTypeId(argArray))))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedArrayLike, u"Reflect.construct");
        }

        return JavascriptFunction::ConstructHelper(VarTo<RecyclableObject>(target), thisArg, newTarget, argArray, scriptContext);
    }
}
