//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#ifdef PERF_COUNTERS

// Forward declaration from perflib.h
struct _PERF_COUNTERSET_INSTANCE;
typedef struct _PERF_COUNTERSET_INSTANCE *PPERF_COUNTERSET_INSTANCE;

enum PageAllocatorType;

#define MAX_OBJECT_NAME_PREFIX 1024

namespace PerfCounter
{
    class Provider;
    class InstanceBase
    {
    protected:
        InstanceBase(Provider& provider, GUID const& guid);
        ~InstanceBase();

        bool IsProviderInitialized() const;
        bool Initialize(char16_t const * wszInstanceName, uint32_t id);
        uint32_t * InitializeSharedMemory(uint32_t numCounter, HANDLE& handle);
        uint32_t * OpenSharedMemory(__in_ecount(MAX_OBJECT_NAME_PREFIX) char16_t const wszObjectNamePrefix[MAX_OBJECT_NAME_PREFIX], uint32_t pid, uint32_t numCounter, HANDLE& handle);
        void UninitializeSharedMemory(uint32_t * data, HANDLE handle);
        bool IsEnabled() const;
    private:
        Provider& GetProvider() { return provider; }
        PPERF_COUNTERSET_INSTANCE GetData() { return instanceData; }

        Provider& provider;
        GUID const& guid;
        PPERF_COUNTERSET_INSTANCE instanceData;
        friend class Counter;
    };

    class Counter
    {
    public:
        Counter() : count(NULL) {};
        void Initialize(InstanceBase& instance, uint32_t id, uint32_t * count);
        void Uninitialize(InstanceBase& instance, uint32_t id);

        Counter& operator+=(size_t value);
        Counter& operator-=(size_t value);
        Counter& operator++();
        Counter& operator--();
        uint32_t GetValue() { return *count; }

    private:
        /* TODO: 64-bit */
        uint32_t * count;
    };

    class PageAllocatorCounterSetDefinition
    {
    public:
        static uint32_t const MaxCounter = 24;
        static GUID const& GetGuid();
        static Provider& GetProvider();

        static uint GetReservedCounterId(PageAllocatorType type);
        static uint GetCommittedCounterId(PageAllocatorType type);
        static uint GetUsedCounterId(PageAllocatorType type);
    };

    class BasicCounterSetDefinition
    {
    public:
        static uint32_t const MaxCounter = 4;
        static GUID const& GetGuid();
        static Provider& GetProvider();
    };

    class CodeCounterSetDefinition
    {
    public:
        static uint32_t const MaxCounter = 17;
        static GUID const& GetGuid();
        static Provider& GetProvider();
    };

#ifdef HEAP_PERF_COUNTERS
    class HeapCounterSetDefinition
    {
    public:
        static uint32_t const MaxCounter = 2;
        static GUID const& GetGuid();
        static Provider& GetProvider();
    };
#endif

#ifdef RECYCLER_PERF_COUNTERS
    class RecyclerCounterSetDefinition
    {
    public:
        static uint32_t const MaxCounter = 14;
        static GUID const& GetGuid();
        static Provider& GetProvider();
    };
#endif

#ifdef PROFILE_RECYCLER_ALLOC

#define RECYCLER_TRACKER_PERF_COUNTER_TYPE(MACRO) \
    MACRO(JavascriptNumber); \
    MACRO(ConcatString); \
    MACRO(LiteralString); \
    MACRO(SubString); \
    MACRO(PropertyString); \
    MACRO(PropertyRecord); \
    MACRO(DynamicObject); \
    MACRO(CustomExternalObject); \
    MACRO(DynamicType); \
    MACRO(JavascriptFunction); \
    MACRO(JavascriptArray); \
    MACRO(SingleCharString); \
    MACRO(FrameDisplay); \
    MACRO(CompoundString); \
    MACRO(RecyclerWeakReferenceBase); \

#define RECYCLER_TRACKER_ARRAY_PERF_COUNTER_TYPE(MACRO) \
    MACRO(Var); \
    MACRO(char16_t); \

#define RECYCLER_TRACKER_WEAKREF_PERF_COUNTER_TYPE(MACRO) \
    MACRO(PropertyRecord); \
    MACRO(DynamicType); \
    MACRO(PropertyString); \
    MACRO(DynamicObject); \
    MACRO(Type); \

#define DECLARE_RECYCLER_TRACKER_PERF_COUNTER_INDEX(type) \
    static uint const type##CounterIndex; \
    static uint const type##SizeCounterIndex;

#define DECLARE_RECYCLER_TRACKER_ARRAY_PERF_COUNTER_INDEX(type) \
    static uint const type##ArrayCounterIndex; \
    static uint const type##ArraySizeCounterIndex;

#define DECLARE_RECYCLER_TRACKER_WEAKREF_PERF_COUNTER_INDEX(type) \
    static uint const type##WeakRefCounterIndex;

    class RecyclerTrackerCounterSetDefinition
    {
    public:
        static uint32_t const MaxCounter = 46;
        static GUID const& GetGuid();
        static Provider& GetProvider();

        RECYCLER_TRACKER_PERF_COUNTER_TYPE(DECLARE_RECYCLER_TRACKER_PERF_COUNTER_INDEX);
        RECYCLER_TRACKER_ARRAY_PERF_COUNTER_TYPE(DECLARE_RECYCLER_TRACKER_ARRAY_PERF_COUNTER_INDEX);
        RECYCLER_TRACKER_WEAKREF_PERF_COUNTER_TYPE(DECLARE_RECYCLER_TRACKER_WEAKREF_PERF_COUNTER_INDEX);
    };

#undef DECLARE_RECYCLER_TRACKER_PERF_COUNTER_INDEX
#undef DECLARE_RECYCLER_TRACKER_ARRAY_PERF_COUNTER_INDEX
#undef DECLARE_RECYCLER_TRACKER_WEAKREF_PERF_COUNTER_INDEX

#endif
};
#endif
