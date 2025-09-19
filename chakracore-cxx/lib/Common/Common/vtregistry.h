//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#if DBG
typedef JsUtil::BaseDictionary<long, LPCSTR, ArenaAllocator> VtableHashMap;
class VirtualTableRegistry
{
public:
    static void Add(long vtable, LPCSTR className);
    static VtableHashMap * CreateVtableHashMap(ArenaAllocator * allocator);
private:
    struct TableEntry
    {
        long vtable;
        LPCSTR className;
    };
    static TableEntry m_knownVtables[];
    static uint32_t m_knownVtableCount;

};
#endif
