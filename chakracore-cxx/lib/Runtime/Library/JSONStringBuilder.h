//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Js
{

class JSONStringBuilder
{
private:
    ScriptContext* scriptContext;
    const char16_t* endLocation;
    char16_t* currentLocation;
    JSONProperty* jsonContent;
    const char16_t* gap;
    charcount_t gapLength;
    uint32_t indentLevel;

    void AppendGap(uint32_t count);
    void AppendCharacter(char16_t character);
    void AppendBuffer(_In_ const char16_t* buffer, charcount_t length);
    void AppendString(_In_ JavascriptString* str);
    void AppendEscapeSequence(_In_ const char16_t character);
    void EscapeAndAppendString(_In_ JavascriptString* str);
    void AppendObjectString(_In_ JSONObject* valueList);
    void AppendArrayString(_In_ JSONArray* valueArray);
    void AppendJSONPropertyString(_In_ JSONProperty* prop);
public:
    JSONStringBuilder(
        _In_ ScriptContext* scriptContext,
        _In_ JSONProperty* jsonContent,
        _In_ char16_t* buffer,
        charcount_t bufferLength,
        _In_opt_ const char16_t* gap,
        charcount_t gapLength);
    void Build();
};

} // namespace Js
