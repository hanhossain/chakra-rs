//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "JsrtRuntime.h"

class JsrtContext : public FinalizableObject
{
public:
    static JsrtContext *New(JsrtRuntime * runtime);

    Js::ScriptContext * GetScriptContext() const { return this->javascriptLibrary->scriptContext; }
    Js::JavascriptLibrary* GetJavascriptLibrary() const { return this->javascriptLibrary; }
    JsrtRuntime * GetRuntime() const { return this->runtime; }
    void* GetExternalData() const { return this->externalData; }
    void SetExternalData(void * data) { this->externalData = data; }

    static JsrtContext * GetCurrent();
    static bool TrySetCurrent(JsrtContext * context);
    static bool Is(void * ref);

    virtual void Mark(Recycler * recycler) override;
    void OnScriptLoad(Js::JavascriptFunction * scriptFunction, Js::Utf8SourceInfo* utf8SourceInfo, CompileScriptException* compileException);
protected:
    DEFINE_VTABLE_CTOR_NOBASE(JsrtContext);
    JsrtContext(JsrtRuntime * runtime);
    void Link();
    void Unlink();
    void SetJavascriptLibrary(Js::JavascriptLibrary * library);
private:
    typename WriteBarrierFieldTypeTraits<Js::JavascriptLibrary *>::Type javascriptLibrary;

    typename WriteBarrierFieldTypeTraits<JsrtRuntime *>::Type runtime;
    typename WriteBarrierFieldTypeTraits<void*>::Type externalData = nullptr;
    typename WriteBarrierFieldTypeTraits<TaggedPointer<JsrtContext>>::Type previous;
    typename WriteBarrierFieldTypeTraits<TaggedPointer<JsrtContext>>::Type next;
};

