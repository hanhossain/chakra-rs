//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#if ENABLE_PROFILE_INFO
namespace Js
{
    SimpleDataCacheWrapper::SimpleDataCacheWrapper(IActiveScriptDataCache* dataCache) :
        dataCache(dataCache),
        outStream(nullptr),
        inStream(nullptr),
        bytesWrittenInBlock(0),
        blocksWritten(0)
    {
    }

    int32_t SimpleDataCacheWrapper::Close()
    {
        int32_t hr = E_FAIL;
        return hr;
    }

    int32_t SimpleDataCacheWrapper::SaveWriteStream()
    {
        int32_t hr = E_FAIL;
        return hr;
    }

    int32_t SimpleDataCacheWrapper::OpenWriteStream()
    {
        return WriteHeader();
    }

    int32_t SimpleDataCacheWrapper::WriteHeader()
    {
        uint32_t jscriptMajorVersion;
        uint32_t jscriptMinorVersion;
        int32_t hr = E_FAIL;

        IFFAILRET(AutoSystemInfo::GetJscriptFileVersion(&jscriptMajorVersion, &jscriptMinorVersion));

        Assert(this->bytesWrittenInBlock == 0);

        IFFAILRET(Write(jscriptMajorVersion));
        IFFAILRET(Write(jscriptMinorVersion));

        Assert(this->bytesWrittenInBlock == sizeof(uint32_t) * 2);

        return hr;
    }

    int32_t SimpleDataCacheWrapper::ReadHeader()
    {
        uint32_t jscriptMajorVersion;
        uint32_t jscriptMinorVersion;
        int32_t hr = E_FAIL;

        IFFAILRET(AutoSystemInfo::GetJscriptFileVersion(&jscriptMajorVersion, &jscriptMinorVersion));

        uint32_t majorVersion;
        IFFAILRET(Read(&majorVersion));
        if (majorVersion != jscriptMajorVersion)
        {
            return E_FAIL;
        }

        uint32_t minorVersion;
        IFFAILRET(Read(&minorVersion));
        if (minorVersion != jscriptMinorVersion)
        {
            return E_FAIL;
        }

        return hr;
    }

    int32_t SimpleDataCacheWrapper::EnsureWriteStream()
    {
        if (IsWriteStreamOpen())
        {
            return S_OK;
        }

        return OpenWriteStream();
    }

    int32_t SimpleDataCacheWrapper::EnsureReadStream()
    {
        if (IsReadStreamOpen())
        {
            return S_OK;
        }

        return OpenReadStream();
    }

    int32_t SimpleDataCacheWrapper::StartBlock(_In_ BlockType blockType, _In_ uint32_t byteCount)
    {
        int32_t hr = E_FAIL;

        if (this->blocksWritten >= MAX_BLOCKS_ALLOWED)
        {
            return hr;
        }

        IFFAILRET(Write(blockType));
        IFFAILRET(Write(byteCount));

        // Reset the bytes written for the current block
        this->bytesWrittenInBlock = 0;
        this->blocksWritten++;
        return hr;
    }

    int32_t SimpleDataCacheWrapper::OpenReadStream()
    {
        return ReadHeader();
    }

    int32_t SimpleDataCacheWrapper::ResetReadStream()
    {
        int32_t hr = E_FAIL;

        if (IsReadStreamOpen())
        {
        }
        else
        {
            // OpenReadStream opens to the beginning of the stream and consumes the header
            hr = OpenReadStream();
        }

        return hr;
    }

    int32_t SimpleDataCacheWrapper::SeekReadStreamToBlockHelper(_In_ BlockType blockType, _Out_opt_ uint32_t* bytesInBlock)
    {
        int32_t hr;
        BlockType currentBlockType = BlockType_Invalid;
        uint32_t byteCount = 0;

        IFFAILRET(Read(&currentBlockType));
        IFFAILRET(Read(&byteCount));

        if (currentBlockType == blockType)
        {
            if (bytesInBlock != nullptr)
            {
                *bytesInBlock = byteCount;
            }
            return S_OK;
        }
        else if (currentBlockType == BlockType_Invalid || byteCount == 0)
        {
            return E_FAIL;
        }
        return SeekReadStreamToBlockHelper(blockType, bytesInBlock);
    }

    int32_t SimpleDataCacheWrapper::SeekReadStreamToBlock(_In_ BlockType blockType, _Out_opt_ uint32_t* bytesInBlock)
    {
        int32_t hr;

        if (bytesInBlock != nullptr)
        {
            *bytesInBlock = 0;
        }

        IFFAILRET(ResetReadStream());

        // Reset above has moved the seek pointer to just after the header, we're at the first block.
        return SeekReadStreamToBlockHelper(blockType, bytesInBlock);
    }
}
#endif
