//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#ifdef DYNAMIC_PROFILE_STORAGE
class DynamicProfileStorage
{
public:
    static bool Initialize();
    static bool Uninitialize();

    static bool IsEnabled() { return enabled; }
    static bool DoCollectInfo() { return collectInfo; }

    template <typename Fn>
    static Js::SourceDynamicProfileManager * Load(__in_z char16_t const * filename, Fn loadFn);
    static void SaveRecord(__in_z char16_t const * filename, __in_ecount(sizeof(uint32_t) + *record) char const * record);

    static char * AllocRecord(uint32_t bufferSize);
    static void DeleteRecord(__in_ecount(sizeof(uint32_t) + *record) char const * record);
    static char const * GetRecordBuffer(__in_ecount(sizeof(uint32_t) + *record) char const * record);
    static char * GetRecordBuffer(__in_ecount(sizeof(uint32_t) + *record) char * record);
    static uint32_t GetRecordSize(__in_ecount(sizeof(uint32_t) + *record) char const * record);
private:
    static char16_t const * GetMessageType();
    static void ClearInfoMap(bool deleteFileStorage);

    static bool ImportFile(__in_z char16_t const * filename, bool allowNonExistingFile);
    static bool ExportFile(__in_z char16_t const * filename);
    static bool SetupCacheDir(__in_z char16_t const * dirname);
    static void DisableCacheDir();

    static bool CreateCacheCatalog();
    static void ClearCacheCatalog();
    static bool LoadCacheCatalog();
    static bool AppendCacheCatalog(__in_z char16_t const * url);
    static bool AcquireLock();
    static bool ReleaseLock();
    static bool VerifyHeader();

    static bool initialized;
    static bool uninitialized;
    static bool enabled;
    static bool collectInfo;
    static bool useCacheDir;
    static char16_t cacheDrive[_MAX_DRIVE];
    static char16_t cacheDir[_MAX_DIR];
    static char16_t catalogFilename[_MAX_PATH];
    static uint32_t const MagicNumber;
    static uint32_t const FileFormatVersion;
    typedef time_t TimeType;
    static inline TimeType GetCreationTime() { return time(NULL); }
    static TimeType creationTime;
    static int32 lastOffset;
    static HANDLE mutex;
    static CriticalSection cs;
    static uint32_t nextFileId;
    static bool locked;
#if DBG_DUMP
    static bool DoTrace();
#endif
    class StorageInfo
    {
    public:
        void GetFilename(_Out_writes_z_(_MAX_PATH) char16_t filename[_MAX_PATH]) const;
        char const * ReadRecord() const;
        bool WriteRecord(__in_ecount(sizeof(uint32_t) + *record) char const * record) const;
        bool isFileStorage;
        union
        {
            uint32_t fileId;
            char const * record;
        };
    };
    typedef JsUtil::BaseDictionary<char16_t const *, StorageInfo, NoCheckHeapAllocator, PrimeSizePolicy, DefaultComparer, JsUtil::DictionaryEntry> InfoMap;
    static InfoMap infoMap;
};

template <class Fn>
Js::SourceDynamicProfileManager *
DynamicProfileStorage::Load(char16_t const * filename, Fn loadFn)
{
    Assert(DynamicProfileStorage::IsEnabled());
    AutoCriticalSection autocs(&cs);
    if (useCacheDir && AcquireLock())
    {
        LoadCacheCatalog(); // refresh the cache catalog
    }
    StorageInfo * info;
    if (!infoMap.TryGetReference(filename, &info))
    {
        if (useCacheDir)
        {
            ReleaseLock();
        }
#if !DBG || !defined(_M_AMD64)
        char16_t const * messageType = GetMessageType();
        if (messageType)
        {
            Output::Print(u"%s: DynamicProfileStorage: Dynamic Profile Data not found for '%s'\n", messageType, filename);
            Output::Flush();
        }
#endif
        return nullptr;
    }
    char const * record;
    if (info->isFileStorage)
    {
        Assert(useCacheDir);
        Assert(locked);
        record = info->ReadRecord();
        ReleaseLock();
        if (record == nullptr)
        {
#if DBG_DUMP
            if (DynamicProfileStorage::DoTrace())
            {
                Output::Print(u"TRACE: DynamicProfileStorage: Failed to load from cache dir for '%s'", filename);
                Output::Flush();
            }
#endif
            return nullptr;
        }
    }
    else
    {
        record = info->record;
    }
    uint32_t recordSize = GetRecordSize(record);
    if (recordSize == 0) 
    {
        return nullptr;
    }
    Js::SourceDynamicProfileManager * sourceDynamicProfileManager = loadFn(GetRecordBuffer(record), recordSize);
    if (info->isFileStorage)
    {
        // The data is backed by a file, we can delete the memory
        Assert(useCacheDir);
        DeleteRecord(record);
    }
#if DBG_DUMP
    if (DynamicProfileStorage::DoTrace() && sourceDynamicProfileManager)
    {
        Output::Print(u"TRACE: DynamicProfileStorage: Dynamic Profile Data Loaded: '%s'\n", filename);
    }
#endif

    if (sourceDynamicProfileManager == nullptr)
    {
        char16_t const * messageType = GetMessageType();
        if (messageType)
        {
            Output::Print(u"%s: DynamicProfileStorage: Dynamic Profile Data corrupted: '%s'\n", messageType, filename);
            Output::Flush();
            Assert(false);
        }
    }
    return sourceDynamicProfileManager;
}
#endif
