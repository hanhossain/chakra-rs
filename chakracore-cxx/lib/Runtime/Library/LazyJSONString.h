//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Js
{
struct JSONObjectProperty;
struct JSONProperty;
struct JSONArray;

enum class JSONContentType : uint8_t
{
    Array,
    Object,
    Undefined,
    Null,
    True,
    False,
    Number,
    String
};

typedef SListCounted<JSONObjectProperty, Recycler> JSONObject;


struct JSONNumberData
{
    typename WriteBarrierFieldTypeTraits<Var>::Type value;
    typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type string;
};


struct JSONProperty
{
    typename WriteBarrierFieldTypeTraits<JSONContentType>::Type type;
    union
    {
        typename WriteBarrierFieldTypeTraits<JSONNumberData>::Type numericValue;
        typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type stringValue;
        typename WriteBarrierFieldTypeTraits<JSONObject*>::Type obj;
        typename WriteBarrierFieldTypeTraits<JSONArray*>::Type arr;
    };

    JSONProperty()
    {
        memset(this, 0, sizeof(JSONProperty));
    }
    JSONProperty(const JSONProperty& other)
    {
        // Copy the full struct and use "typename WriteBarrierFieldTypeTraits<Var>::Type" to identify write barrier
        // policy as the struct contains Vars
        CopyArray<JSONProperty, typename WriteBarrierFieldTypeTraits<Var>::Type>(this, 1, &other, 1);
    }
};

struct JSONObjectProperty
{
    typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type propertyName;
    typename WriteBarrierFieldTypeTraits<JSONProperty>::Type propertyValue;

    JSONObjectProperty() : propertyName(nullptr), propertyValue()
    {
    }
    JSONObjectProperty(const JSONObjectProperty& other) :
        propertyName(other.propertyName),
        propertyValue(other.propertyValue)
    {
    }
};

struct JSONArray
{
    typename WriteBarrierFieldTypeTraits<uint32_t>::Type length;
    typename WriteBarrierFieldTypeTraits<JSONProperty>::Type arr[];
};

class LazyJSONString : public JavascriptString
{
private:
    typename WriteBarrierFieldTypeTraits<charcount_t>::Type gapLength;
    typename WriteBarrierFieldTypeTraits<JSONProperty*>::Type jsonContent;
    typename WriteBarrierFieldTypeTraits<const char16_t*>::Type gap;

    DynamicObject* ReconstructObject(_In_ JSONObject* valueList) const;
    JavascriptArray* ReconstructArray(_In_ JSONArray* valueArray) const;
    Var ReconstructVar(_In_ JSONProperty* content) const;


    static const char16_t escapeMap[128];
public:
    static const uint8_t escapeMapCount[128];

protected:
    DEFINE_VTABLE_CTOR(LazyJSONString, JavascriptString);

public:
    LazyJSONString(_In_ JSONProperty* content, charcount_t length, _In_opt_ const char16_t* gap, charcount_t gapLength, _In_ StaticType* type);
    Var TryParse() const;

    // Tells if the string has a gap with characters that might impact JSON.parse
    bool HasComplexGap() const;

    const char16_t* GetSz() override;

    virtual VTableValue DummyVirtualFunctionToHinderLinkerICF()
    {
        return VTableValue::VtableLazyJSONString;
    }

}; // class LazyJSONString

template <> bool VarIsImpl<LazyJSONString>(RecyclableObject* obj);

} // namespace Js
