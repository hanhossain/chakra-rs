//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{

class JavascriptAsyncGeneratorFunction : public JavascriptGeneratorFunction
{
  private:
      static FunctionInfo functionInfo;

      DEFINE_VTABLE_CTOR(JavascriptAsyncGeneratorFunction, JavascriptGeneratorFunction);
      friend class Js::CrossSiteObject<JavascriptAsyncGeneratorFunction>;
      void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
      {
          Assert(this->GetScriptContext() != scriptContext);
          AssertMsg(VirtualTableInfo<JavascriptAsyncGeneratorFunction>::HasVirtualTable(this),
                    "Derived class need to define marshal to script context");
          VirtualTableInfo<Js::CrossSiteObject<JavascriptAsyncGeneratorFunction>>::SetVirtualTable(this);
      };

  protected:
      JavascriptAsyncGeneratorFunction(DynamicType* type);

  public:
      JavascriptAsyncGeneratorFunction(
          DynamicType* type,
          GeneratorVirtualScriptFunction* scriptFunction);

      static JavascriptAsyncGeneratorFunction* New(
          ScriptContext* scriptContext,
          GeneratorVirtualScriptFunction* scriptFunction);

      static Var EntryAsyncGeneratorFunctionImplementation(
          RecyclableObject* function,
          CallInfo callInfo, ...);

      inline static bool Test(JavascriptFunction *obj)
      {
          return
              VirtualTableInfo<JavascriptAsyncGeneratorFunction>::HasVirtualTable(obj) ||
              VirtualTableInfo<CrossSiteObject<JavascriptAsyncGeneratorFunction>>::HasVirtualTable(obj);
      }

  public:
      VTableValue DummyVirtualFunctionToHinderLinkerICF() const override
      {
          return VTableValue::VtableJavascriptAsyncGeneratorFunction;
      }
};

template<>
bool VarIsImpl<JavascriptAsyncGeneratorFunction>(RecyclableObject* obj);

}
