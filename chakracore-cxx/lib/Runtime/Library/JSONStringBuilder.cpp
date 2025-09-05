//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "RuntimeLibraryPch.h"

namespace Js
{

void
JSONStringBuilder::AppendCharacter(char16_t character)
{
    AssertOrFailFast(this->currentLocation < endLocation);
    *this->currentLocation = character;
    ++this->currentLocation;
}

void
JSONStringBuilder::AppendBuffer(_In_ const char16_t* buffer, charcount_t length)
{
    AssertOrFailFast(this->currentLocation + length <= endLocation);
    wmemcpy_s(this->currentLocation, length, buffer, length);
    this->currentLocation += length;
}

void
JSONStringBuilder::AppendString(_In_ JavascriptString* str)
{
    AppendBuffer(str->GetString(), str->GetLength());
}

void
JSONStringBuilder::AppendEscapeSequence(_In_ const char16_t character)
{
    // Convert character into a 4 digit hex code (e.g. \u0010)
    this->AppendCharacter(u'\\');
    this->AppendCharacter(u'u');
    {
        char16_t buf[5];
        // Get hex value
        _ltow_s(character, buf, _countof(buf), 16);

        // Append leading zeros if necessary before the hex value
        charcount_t count = static_cast<charcount_t>(wcslen(buf));
        switch (count)
        {
        case 1:
            this->AppendCharacter(u'0');
        case 2:
            this->AppendCharacter(u'0');
        case 3:
            this->AppendCharacter(u'0');
        default:
            this->AppendBuffer(buf, count);
            break;
        }
    }
}

void
JSONStringBuilder::EscapeAndAppendString(_In_ JavascriptString* str)
{
    const charcount_t strLength = str->GetLength();

    // Strings should be surrounded by double quotes
    this->AppendCharacter(u'"');
    const char16_t* bufferStart = str->GetString();
    for (const char16_t* index = bufferStart; index < bufferStart + strLength; ++index)
    {
        char16_t currentCharacter = *index;
        switch (currentCharacter)
        {
        case u'"':
        case u'\\':
            // Special characters are escaped with a backslash
            this->AppendCharacter(u'\\');
            this->AppendCharacter(currentCharacter);
            break;
        case u'\b':
            this->AppendCharacter(u'\\');
            this->AppendCharacter(u'b');
            break;
        case u'\f':
            this->AppendCharacter(u'\\');
            this->AppendCharacter(u'f');
            break;
        case u'\n':
            this->AppendCharacter(u'\\');
            this->AppendCharacter(u'n');
            break;
        case u'\r':
            this->AppendCharacter(u'\\');
            this->AppendCharacter(u'r');
            break;
        case u'\t':
            this->AppendCharacter(u'\\');
            this->AppendCharacter(u't');
            break;
        default:
            if (currentCharacter < u' ' || utf8::IsLowSurrogateChar(currentCharacter))
            {
                this->AppendEscapeSequence(currentCharacter);
            }
            else if (utf8::IsHighSurrogateChar(currentCharacter))
            {
                if (index + 1 < bufferStart + strLength && utf8::IsLowSurrogateChar(*(index + 1)))
                {
                    // Append surrogate pair normally
                    this->AppendCharacter(currentCharacter);
                    this->AppendCharacter(*(index + 1));

                    // Skip the trailing-surrogate code unit
                    index++;
                }
                else
                {
                    // High-surrogate code unit not followed by a trailing-surrogate code unit should be escaped.
                    this->AppendEscapeSequence(currentCharacter);
                }
            }
            else
            {
                this->AppendCharacter(currentCharacter);
            }
            break;
        }
    }

    this->AppendCharacter(u'"');
}

void
JSONStringBuilder::AppendGap(uint32 count)
{
    for (uint i = 0; i < count; ++i)
    {
        this->AppendBuffer(this->gap, this->gapLength);
    }
}

void
JSONStringBuilder::AppendObjectString(_In_ JSONObject* valueList)
{
    const uint elementCount = valueList->Count();
    if (elementCount == 0)
    {
        this->AppendCharacter(u'{');
        this->AppendCharacter(u'}');
        return;
    }

    const uint32 stepbackLevel = this->indentLevel;
    ++this->indentLevel;

    this->AppendCharacter(u'{');
    if (this->gap != nullptr)
    {
        this->AppendCharacter(u'\n');
        this->AppendGap(indentLevel);
    }

    bool isFirstMember = true;
    FOREACH_SLISTCOUNTED_ENTRY(JSONObjectProperty, entry, valueList)
    {
        if (!isFirstMember)
        {
            if (this->gap == nullptr)
            {
                this->AppendCharacter(u',');
            }
            else
            {
                this->AppendCharacter(u',');
                this->AppendCharacter(u'\n');
                this->AppendGap(indentLevel);
            }
        }
        this->EscapeAndAppendString(entry.propertyName);
        this->AppendCharacter(u':');
        if (this->gap != nullptr)
        {
            this->AppendCharacter(u' ');
        }

        this->AppendJSONPropertyString(&entry.propertyValue);

        isFirstMember = false;
    }
    NEXT_SLISTCOUNTED_ENTRY;

    if (this->gap != nullptr)
    {
        this->AppendCharacter(u'\n');
        this->AppendGap(stepbackLevel);
    }

    this->AppendCharacter(u'}');

    this->indentLevel = stepbackLevel;
}

void
JSONStringBuilder::AppendArrayString(_In_ JSONArray* valueArray)
{
    uint32 length = valueArray->length;
    if (length == 0)
    {
        this->AppendCharacter(u'[');
        this->AppendCharacter(u']');
        return;
    }

    const uint32 stepbackLevel = this->indentLevel;
    ++this->indentLevel;

    JSONProperty* arr = valueArray->arr;

    this->AppendCharacter(u'[');

    if (this->gap != nullptr)
    {
        this->AppendCharacter(u'\n');
        this->AppendGap(indentLevel);
    }

    this->AppendJSONPropertyString(&arr[0]);

    for (uint32 i = 1; i < length; ++i)
    {
        if (this->gap == nullptr)
        {
            this->AppendCharacter(u',');
        }
        else
        {
            this->AppendCharacter(u',');
            this->AppendCharacter(u'\n');
            this->AppendGap(indentLevel);
        }
        AppendJSONPropertyString(&arr[i]);
    }

    if (this->gap != nullptr)
    {
        this->AppendCharacter(u'\n');
        this->AppendGap(stepbackLevel);
    }
    this->AppendCharacter(u']');

    this->indentLevel = stepbackLevel;
}

void
JSONStringBuilder::AppendJSONPropertyString(_In_ JSONProperty* prop)
{
    switch (prop->type)
    {
    case JSONContentType::False:
        this->AppendString(this->scriptContext->GetLibrary()->GetFalseDisplayString());
        return;
    case JSONContentType::True:
        this->AppendString(this->scriptContext->GetLibrary()->GetTrueDisplayString());
        return;
    case JSONContentType::Null:
        this->AppendString(this->scriptContext->GetLibrary()->GetNullDisplayString());
        return;
    case JSONContentType::Number:
        this->AppendString(prop->numericValue.string);
        return;
    case JSONContentType::Object:
        this->AppendObjectString(prop->obj);
        return;
    case JSONContentType::Array:
        this->AppendArrayString(prop->arr);
        return;
    case JSONContentType::String:
        this->EscapeAndAppendString(prop->stringValue);
        return;
    default:
        Assume(UNREACHED);
    }
}

void
JSONStringBuilder::Build()
{
    this->AppendJSONPropertyString(this->jsonContent);
    // Null terminate the string
    AssertOrFailFast(this->currentLocation == endLocation);
    *this->currentLocation = u'\0';
}

JSONStringBuilder::JSONStringBuilder(
    _In_ ScriptContext* scriptContext,
    _In_ JSONProperty* jsonContent,
    _In_ char16_t* buffer,
    charcount_t bufferLength,
    _In_opt_ const char16_t* gap,
    charcount_t gapLength) :
        scriptContext(scriptContext),
        endLocation(buffer + bufferLength - 1),
        currentLocation(buffer),
        jsonContent(jsonContent),
        gap(gap),
        gapLength(gapLength),
        indentLevel(0)
{
}

} //namespace Js
