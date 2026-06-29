//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

class HeapConstants
{
public:
    static const uint MaxSmallObjectSize = 768;
    static const uint MaxMediumObjectSize = 8 * 1024; // Maximum medium object size is 8K
    static const uint ObjectAllocationShift = 4;        // 16
    static const uint ObjectGranularity = 1 << ObjectAllocationShift;
    static const uint BucketCount = (MaxSmallObjectSize >> ObjectAllocationShift);
    static const uint MediumObjectGranularity = 256;
    static const uint MediumBucketCount = (MaxMediumObjectSize - MaxSmallObjectSize) / MediumObjectGranularity;
};

///
/// BlockAttributes are used to determine the allocation characteristics of a heap block
/// These include the number of pages to allocate, the object capacity of the block
/// and the shape of the object's bit vectors
///
class SmallAllocationBlockAttributes
{
public:
    static const size_t MinObjectSize = HeapConstants::ObjectGranularity;

    static const size_t PageCount = 1;
    static const size_t BitVectorCount = ((PageCount * AutoSystemInfo::PageSize) / HeapConstants::ObjectGranularity);
    static const ushort MaxAddressBit = BitVectorCount - 1;
    static const uint   BucketCount = HeapConstants::BucketCount;
    static const size_t BucketGranularity = HeapConstants::ObjectGranularity;
    static const uint   MaxObjectSize = HeapConstants::MaxSmallObjectSize;
    static const uint   MaxObjectCount = PageCount * AutoSystemInfo::PageSize / HeapConstants::ObjectGranularity;
    static const uint   MaxSmallObjectCount = MaxObjectCount;
    static const uint   ObjectCountPerPage = AutoSystemInfo::PageSize / HeapConstants::ObjectGranularity;

    // This is there for RecyclerSweep to distinguish which bucket index to use
    static const bool IsSmallBlock = true;
    static const bool IsMediumBlock = false;
    static const bool IsLargeBlock = false;

    static BOOL IsAlignedObjectSize(size_t sizeCat);
};

class MediumAllocationBlockAttributes
{
public:
    static const size_t PageCount = 8;
    static const size_t MinObjectSize = HeapConstants::MaxSmallObjectSize;
    static const ushort BitVectorCount = ((PageCount * AutoSystemInfo::PageSize) / HeapConstants::ObjectGranularity);
    static const size_t MaxAddressBit = (BitVectorCount - 1);
    static const uint   MaxObjectSize = HeapConstants::MaxMediumObjectSize;
    static const uint   BucketCount = HeapConstants::MediumBucketCount;
    static const size_t BucketGranularity = HeapConstants::MediumObjectGranularity;
    static const uint   MaxObjectCount = PageCount * AutoSystemInfo::PageSize / (MinObjectSize + BucketGranularity);
    static const uint   MaxSmallObjectCount = PageCount * AutoSystemInfo::PageSize / HeapConstants::ObjectGranularity;
    static const uint   ObjectCountPerPage = AutoSystemInfo::PageSize / MinObjectSize;

    // This is there for RecyclerSweep to distinguish which bucket index to use
    static const bool IsSmallBlock = false;
    static const bool IsMediumBlock = true;
    static const bool IsLargeBlock = false;

    static BOOL IsAlignedObjectSize(size_t sizeCat);
};

class LargeAllocationBlockAttributes
{
public:
    // This is there for RecyclerSweep to distinguish which bucket index to use
    static const bool IsSmallBlock = false;
    static const bool IsMediumBlock = false;
    static const bool IsLargeBlock = true;
};
