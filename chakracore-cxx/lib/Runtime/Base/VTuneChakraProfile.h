//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#ifdef VTUNE_PROFILING

//
// Encapsulates all VTune Chakra profiling event logging and registration etc..
//
class VTuneChakraProfile
{
public:
    static const utf8char_t DynamicCode[];

private:
    static utf8char_t* GetUrl(Js::FunctionBody* body, size_t* urlLength);
};

#endif
