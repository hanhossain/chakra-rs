set (CCM_SOURCE_FILES ${CCM_SOURCE_FILES}
    Allocator.cpp
    ArenaAllocator.cpp
    CustomHeap.cpp
    CommonMemoryPch.cpp
    DelayDeletingFunctionTable.cpp
    EtwMemoryTracking.cpp
    ForcedMemoryConstraints.cpp
    HeapAllocator.cpp
    HeapAllocatorOperators.cpp
    HeapBlock.cpp
    HeapBlockMap.cpp
    HeapBucket.cpp
    HeapInfo.cpp
    HeapInfoManager.cpp
    IdleDecommitPageAllocator.cpp
    LargeHeapBlock.cpp
    LargeHeapBucket.cpp
    LeakReport.cpp
    MarkContext.cpp
    MemoryLogger.cpp
    MemoryTracking.cpp
    PageAllocator.cpp
    Recycler.cpp
    RecyclerHeuristic.cpp
    RecyclerObjectDumper.cpp
    RecyclerObjectGraphDumper.cpp
    RecyclerPageAllocator.cpp
    RecyclerSweep.cpp
    RecyclerSweepManager.cpp
    RecyclerWriteBarrierManager.cpp
    SmallFinalizableHeapBlock.cpp
    SmallFinalizableHeapBucket.cpp
    SmallHeapBlockAllocator.cpp
    SmallLeafHeapBlock.cpp
    SmallLeafHeapBucket.cpp
    SmallNormalHeapBlock.cpp
    SmallNormalHeapBucket.cpp
    StressTest.cpp
    VirtualAllocWrapper.cpp
    )

if(CC_TARGETS_AMD64)
    set (CCM_SOURCE_FILES ${CCM_SOURCE_FILES}
        amd64/XDataAllocator.cpp
        amd64/amd64_SAVE_REGISTERS.S
    )
elseif(CC_TARGETS_ARM64)
    set (CCM_SOURCE_FILES ${CCM_SOURCE_FILES}
        arm64/XDataAllocator.cpp
        arm64/arm64_SAVE_REGISTERS.S
    )
endif()

add_library (Chakra.Common.Memory OBJECT
    ${CCM_SOURCE_FILES}
    )

# TODO (hanhossain): remove most directory paths
target_include_directories (Chakra.Common.Memory PUBLIC
#    Chakra.Common.Memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
#    ${CHAKRACORE_SOURCE_DIR}/pal/src
#    ${CHAKRACORE_SOURCE_DIR}/pal/src/include
#    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
#    ${CHAKRACORE_SOURCE_DIR}/bin
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
#    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
#    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/PlatformAgnostic
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Math
)
