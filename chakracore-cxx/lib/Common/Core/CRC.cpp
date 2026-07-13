//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "CommonCorePch.h"
#include "Core/CRC.h"

unsigned int CalculateCRC32(unsigned int bufferCRC, size_t data)
{
    /* update running CRC calculation with contents of a buffer */

    bufferCRC = bufferCRC ^ 0xffffffffL;
    bufferCRC = crc_32_tab[(bufferCRC ^ data) & 0xFF] ^ (bufferCRC >> 8);
    return (bufferCRC ^ 0xffffffffL);
}

unsigned int CalculateCRC32(const char* in)
{
    unsigned int crc = static_cast<unsigned int>(-1);
    while (*in != '\0')
    {
        crc = (crc >> 8) ^ crc_32_tab[(crc ^ *in) & 0xFF];
        in++;
    }
    return crc ^ static_cast<unsigned int>(-1);
}

uint CalculateCRC(uint bufferCRC, size_t data)
{
#if defined(__SSE4_2__)
#if defined(_M_X64)
    if (AutoSystemInfo::Data.SSE4_2Available())
    {
        //CRC32 always returns a 32-bit result
        return static_cast<uint>(_mm_crc32_u64(bufferCRC, data));
    }
#endif
#endif
    return CalculateCRC32(bufferCRC, data);
}

uint CalculateCRC(uint bufferCRC, size_t count, _In_reads_bytes_(count) void * buffer)
{
    for (uint index = 0; index < count; index++)
    {
        bufferCRC = CalculateCRC(bufferCRC, *(static_cast<uint8_t*>(buffer) + index));
    }
    return bufferCRC;
}
