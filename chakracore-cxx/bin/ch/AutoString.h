#pragma once
#include "stdafx.h"

class AutoString
{
    size_t length_;
    char* data_;
    char16_t* data_wide;
    JsErrorCode errorCode_;
    bool dontFree_;
public:
    AutoString():length_(0), data_(nullptr),
        data_wide(nullptr), errorCode_(JsNoError), dontFree_(false)
    { }

    AutoString(AutoString &autoString):length_(autoString.length_),
        data_(autoString.data_), data_wide(autoString.data_wide),
        errorCode_(JsNoError), dontFree_(false)
    {
        autoString.dontFree_ = true; // take over the ownership
    }

    AutoString(JsValueRef value):length_(0), data_(nullptr),
        data_wide(nullptr), errorCode_(JsNoError), dontFree_(false)
    {
        Initialize(value);
    }

    JsErrorCode Initialize(JsValueRef value)
    {
        errorCode_ = JsNoError;
        JsValueRef strValue;
        JsValueType type;
        ChakraRTInterface::JsGetValueType(value, &type);
        if (type != JsString)
        {
            errorCode_ = ChakraRTInterface::JsConvertValueToString(value, &strValue);
        }
        else
        {
            strValue = value;
        }
        size_t length = 0;
        if (errorCode_ == JsNoError)
        {
            errorCode_ = ChakraRTInterface::JsCopyString(strValue, nullptr, 0, &length);
            if (errorCode_ == JsNoError)
            {
                data_ = (char*)malloc((length + 1) * sizeof(char));
                size_t writtenLength = 0;
                errorCode_ = ChakraRTInterface::JsCopyString(strValue, data_, length, &writtenLength);
                if (errorCode_ == JsNoError)
                {
                    AssertMsg(length == writtenLength, "Inconsistent length in utf8 encoding");
                }
            }
        }
        if (errorCode_ == JsNoError)
        {
            *(data_ + length) = char(0);
            this->length_ = length;
        }
        return errorCode_;
    }

    void MakePersistent()
    {
        dontFree_ = true;
    }

    const char * GetString() const {
        return data_;
    }

    char16_t* GetWideString(charcount_t* destCount = nullptr)
    {
        if(data_wide || !data_)
        {
            return data_wide;
        }
        charcount_t tempDestCount;
        utf8::NarrowStringToWide<utf8::malloc_allocator>(data_, length_, &data_wide, &tempDestCount);

        if (destCount)
        {
            *destCount = tempDestCount;
        }
        return data_wide;
    }

    bool HasError() const {
        return errorCode_ != JsNoError;
    }

    JsErrorCode GetError() const {
        return errorCode_;
    }

    size_t GetLength() const {
        return length_;
    }

    ~AutoString()
    {
        // we need persistent source string
        // for externalArrayBuffer source
        // externalArrayBuffer finalize should
        // free this memory
        if (!dontFree_ && data_ != nullptr)
        {
            free(data_);
            data_ = nullptr;
        }

        // Free this anyway.
        if (data_wide != nullptr)
        {
            free(data_wide);
            data_wide = nullptr;
        }
    }

    char* operator*() { return data_; }
};

