#pragma once

#include "PlatformAgnostic/CommonTypedefs.h"

class BailOutRecord;

struct LazyBailOutRecord
{
    uint32 offset;
    BailOutRecord* bailOutRecord;

    LazyBailOutRecord() : offset(0), bailOutRecord(nullptr) {}

    LazyBailOutRecord(uint32 offset, BailOutRecord* record) :
        offset(offset), bailOutRecord(record)
    {}

#if DBG
    void Dump(Js::FunctionBody* functionBody) const;
#endif
};
