//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include <limits>
#include <sys/stat.h>

#define MAX_URI_LENGTH 512

#define TTD_MAX_FILE_LENGTH MAX_URI_LENGTH
#define TTD_HOST_PATH_SEP "/"

void TTDHostBuildCurrentExeDirectory(char* path, size_t* pathLength, size_t bufferLength)
{
    char exePath[TTD_MAX_FILE_LENGTH];
    PlatformAgnostic::SystemInfo::GetBinaryLocation(exePath, TTD_MAX_FILE_LENGTH);

    size_t i = strlen(exePath) - 1;
    while (exePath[i] != TTD_HOST_PATH_SEP[0] && i != 0)
    {
        --i;
    }
    if (i == 0)
    {
        fwprintf(stderr, u"Can't get current exe directory");
        exit(1);
    }
    if (i + 2 > bufferLength)
    {
        fwprintf(stderr, u"Don't overflow path buffer during copy");
        exit(1);
    }
    memcpy_s(path, bufferLength, exePath, i + 1);
    *pathLength = i + 1;
    path[*pathLength] = '\0';
}

int TTDHostMKDir(const char* path, size_t pathLength)
{
    return mkdir(path, 0700);
}

JsTTDStreamHandle TTDHostOpen(size_t pathLength, const char* path, bool isWrite)
{
    return (JsTTDStreamHandle)fopen(path, isWrite ? "w+b" : "r+b");
}

#define TTDHostRead(buff, size, handle) fread(buff, 1, size, (FILE*)handle)
#define TTDHostWrite(buff, size, handle) fwrite(buff, 1, size, (FILE*)handle)

int GetPathNameLocation(const char * filename)
{
    int filenameLength = (int) strlen(filename);
    int pos;

    if (filenameLength <= 0)
    {
        return -1;
    }

    for (pos = filenameLength - 1; pos >= 0; pos--)
    {
        char ch = filename[pos];
        if (ch == '/' || ch == '\\') break;
    }

    return pos;
}

inline void pathcpy(char * target, const char * src, uint length)
{
    for (int i = 0; i < length; i++)
    {
        if (src[i] == '\\')
        {
            target[i] = '/';
        }
        else
        {
            target[i] = src[i];
        }
    }
}

uint ConcatPath(const char * filenameLeft, uint posPathSep, const char * filenameRight, char* buffer, uint bufferLength)
{
    int filenameRightLength = (int) strlen(filenameRight);

    // [ path[/] ] + [filename] + /0
    uint totalLength = posPathSep + filenameRightLength + 1;
    if (buffer == nullptr)
    {
        return totalLength;
    }

    if (bufferLength < totalLength)
    {
        fprintf(stderr, "Error: file path is too long.\n");
        return (uint)-1;
    }

    pathcpy(buffer, filenameLeft, posPathSep);
    buffer += posPathSep;
    pathcpy(buffer, filenameRight, filenameRightLength);
    buffer += filenameRightLength;
    buffer[0] = char(0);
    return totalLength;
}

int32_t Helpers::LoadScriptFromFile(const char * filenameToLoad, const char *& contents, uint32_t* lengthBytesOut /*= nullptr*/, std::string* fullPath /*= nullptr*/, bool shouldMute /*=false */)
{
    static char sHostApplicationPathBuffer[MAX_URI_LENGTH];
    static uint sHostApplicationPathBufferLength = (uint) -1;
    char combinedPathBuffer[MAX_URI_LENGTH];

    int32_t hr = S_OK;
    uint8_t * pRawBytes = nullptr;
    uint8_t * pRawBytesFromMap = nullptr;
    uint32_t lengthBytes = 0;
    contents = nullptr;
    FILE * file = NULL;
    size_t bufferLength = 0;

    const char * filename = fullPath == nullptr ? filenameToLoad : (const char *)(fullPath->c_str());
    if (sHostApplicationPathBufferLength == (uint)-1)
    {
        // consider incoming filename as the host app and base its' path for others
        sHostApplicationPathBufferLength = GetPathNameLocation(filename);
        if (sHostApplicationPathBufferLength == -1)
        {
            // host app has no path info. (it must be located on current folder!)
            sHostApplicationPathBufferLength = 0;
        }
        else
        {
            sHostApplicationPathBufferLength += 1;
            Assert(sHostApplicationPathBufferLength < MAX_URI_LENGTH);
            // save host app's path and fix the path separator for platform
            pathcpy(sHostApplicationPathBuffer, filename, sHostApplicationPathBufferLength);
        }
        sHostApplicationPathBuffer[sHostApplicationPathBufferLength] = char(0);
    }
    else if (filename[0] != '/' && filename[0] != '\\' && fullPath == nullptr) // make sure it's not a full path
    {
        // concat host path and filename
        uint len = ConcatPath(sHostApplicationPathBuffer, sHostApplicationPathBufferLength,
                   filename, combinedPathBuffer, MAX_URI_LENGTH);

        if (len == (uint)-1)
        {
            hr = E_FAIL;
            goto Error;
        }
        filename = combinedPathBuffer;
    }

    // check if have it registered
    AutoString *data;
    if (SourceMap::Find(filenameToLoad, strlen(filenameToLoad), &data) ||
        SourceMap::Find(filename, strlen(filename), &data))
    {
        pRawBytesFromMap = (uint8_t*) data->GetString();
        lengthBytes = (uint32_t) data->GetLength();
    }
    else
    {
        // Open the file as a binary file to prevent CRT from handling encoding, line-break conversions,
        // etc.
        if (fopen_s(&file, filename, "rb") != 0)
        {
            if (!HostConfigFlags::flags.MuteHostErrorMsgIsEnabled && !shouldMute)
            {
// TODO (hanhossain): may want to include <unistd.h> to import _POSIX_VERSION
#if defined(_POSIX_VERSION)
                fprintf(stderr, "Error in opening file: ");
                perror(filename);
#endif
            }

            IfFailGo(E_FAIL);
        }
    }

    if (file != NULL)
    {
        // Determine the file length, in bytes.
        fseek(file, 0, SEEK_END);
        lengthBytes = ftell(file);
        fseek(file, 0, SEEK_SET);
    }

    if (lengthBytes != 0)
    {
        bufferLength = lengthBytes + sizeof(uint8_t);
        pRawBytes = (uint8_t *)malloc(bufferLength);
    }
    else
    {
        bufferLength = 1;
        pRawBytes = (uint8_t *)malloc(bufferLength);
    }

    if (nullptr == pRawBytes)
    {
        fwprintf(stderr, u"out of memory");
        IfFailGo(E_OUTOFMEMORY);
    }

    if (lengthBytes != 0)
    {
        if (file != NULL)
        {
            //
            // Read the entire content as a binary block.
            //
            size_t readBytes = fread(pRawBytes, sizeof(uint8_t), lengthBytes, file);
            if (readBytes < lengthBytes * sizeof(uint8_t))
            {
                IfFailGo(E_FAIL);
            }
        }
        else // from module source register
        {
            // Q: module source is on persistent memory. Why do we use the copy instead?
            // A: if we use the same memory twice, ch doesn't know that during FinalizeCallback free.
            // the copy memory will be freed by the finalizer
            Assert(pRawBytesFromMap);
            memcpy_s(pRawBytes, bufferLength, pRawBytesFromMap, lengthBytes);
        }
    }

    if (pRawBytes)
    {
        pRawBytes[lengthBytes] = 0; // Null terminate it. Could be UTF16
    }

    if (file != NULL)
    {
        //
        // Read encoding to make sure it's supported
        //
        // Warning: The UNICODE buffer for parsing is supposed to be provided by the host.
        // This is not a complete read of the encoding. Some encodings like UTF7, UTF1, EBCDIC, SCSU, BOCU could be
        // wrongly classified as ANSI
        //
#pragma warning(push)
        // suppressing prefast warning that "readable size is bufferLength
        // bytes but 2 may be read" as bufferLength is clearly > 2 in the code that follows
#pragma warning(disable:6385)
        C_ASSERT(sizeof(char16_t) == 2);
        if (bufferLength > 2)
        {
            __analysis_assume(bufferLength > 2);
#pragma prefast(push)
#pragma prefast(disable:6385, "PREfast incorrectly reports this as an out-of-bound access.");
            if ((pRawBytes[0] == 0xFE && pRawBytes[1] == 0xFF) ||
                (pRawBytes[0] == 0xFF && pRawBytes[1] == 0xFE) ||
                (bufferLength > 4 && pRawBytes[0] == 0x00 && pRawBytes[1] == 0x00 &&
                    ((pRawBytes[2] == 0xFE && pRawBytes[3] == 0xFF) ||
                    (pRawBytes[2] == 0xFF && pRawBytes[3] == 0xFE))))

            {
                // unicode unsupported
                fwprintf(stderr, u"unsupported file encoding. Only ANSI and UTF8 supported");
                IfFailGo(E_UNEXPECTED);
            }
#pragma prefast(pop)
        }
#pragma warning(pop)
    }

    contents = reinterpret_cast<const char *>(pRawBytes);

Error:
    if (SUCCEEDED(hr))
    {
        if (lengthBytesOut)
        {
            *lengthBytesOut = lengthBytes;
        }
    }

    if (file != NULL)
    {
        fclose(file);
    }

    if (pRawBytes && reinterpret_cast<const char *>(pRawBytes) != contents)
    {
        free(pRawBytes);
    }

    return hr;
}

const char16_t* Helpers::JsErrorCodeToString(JsErrorCode jsErrorCode)
{
    bool hasException = false;
    ChakraRTInterface::JsHasException(&hasException);
    if (hasException)
    {
        WScriptJsrt::PrintException("", JsErrorScriptException);
    }

    switch (jsErrorCode)
    {
    case JsNoError:                            return u"JsNoError";
    // JsErrorCategoryUsage
    case JsErrorCategoryUsage:                 return u"JsErrorCategoryUsage";
    case JsErrorInvalidArgument:               return u"JsErrorInvalidArgument";
    case JsErrorNullArgument:                  return u"JsErrorNullArgument";
    case JsErrorNoCurrentContext:              return u"JsErrorNoCurrentContext";
    case JsErrorInExceptionState:              return u"JsErrorInExceptionState";
    case JsErrorNotImplemented:                return u"JsErrorNotImplemented";
    case JsErrorWrongThread:                   return u"JsErrorWrongThread";
    case JsErrorRuntimeInUse:                  return u"JsErrorRuntimeInUse";
    case JsErrorBadSerializedScript:           return u"JsErrorBadSerializedScript";
    case JsErrorInDisabledState:               return u"JsErrorInDisabledState";
    case JsErrorCannotDisableExecution:        return u"JsErrorCannotDisableExecution";
    case JsErrorHeapEnumInProgress:            return u"JsErrorHeapEnumInProgress";
    case JsErrorArgumentNotObject:             return u"JsErrorArgumentNotObject";
    case JsErrorInProfileCallback:             return u"JsErrorInProfileCallback";
    case JsErrorInThreadServiceCallback:       return u"JsErrorInThreadServiceCallback";
    case JsErrorCannotSerializeDebugScript:    return u"JsErrorCannotSerializeDebugScript";
    case JsErrorAlreadyDebuggingContext:       return u"JsErrorAlreadyDebuggingContext";
    case JsErrorAlreadyProfilingContext:       return u"JsErrorAlreadyProfilingContext";
    case JsErrorIdleNotEnabled:                return u"JsErrorIdleNotEnabled";
    case JsErrorInObjectBeforeCollectCallback: return u"JsErrorInObjectBeforeCollectCallback";
    case JsErrorObjectNotInspectable:          return u"JsErrorObjectNotInspectable";
    case JsErrorPropertyNotSymbol:             return u"JsErrorPropertyNotSymbol";
    case JsErrorPropertyNotString:             return u"JsErrorPropertyNotString";
    case JsErrorInvalidContext:                return u"JsErrorInvalidContext";
    case JsInvalidModuleHostInfoKind:          return u"JsInvalidModuleHostInfoKind";
    case JsErrorModuleParsed:                  return u"JsErrorModuleParsed";
    // JsErrorCategoryEngine
    case JsErrorCategoryEngine:                return u"JsErrorCategoryEngine";
    case JsErrorOutOfMemory:                   return u"JsErrorOutOfMemory";
    case JsErrorBadFPUState:                   return u"JsErrorBadFPUState";
    // JsErrorCategoryScript
    case JsErrorCategoryScript:                return u"JsErrorCategoryScript";
    case JsErrorScriptException:               return u"JsErrorScriptException";
    case JsErrorScriptCompile:                 return u"JsErrorScriptCompile";
    case JsErrorScriptTerminated:              return u"JsErrorScriptTerminated";
    case JsErrorScriptEvalDisabled:            return u"JsErrorScriptEvalDisabled";
    // JsErrorCategoryFatal
    case JsErrorCategoryFatal:                 return u"JsErrorCategoryFatal";
    case JsErrorFatal:                         return u"JsErrorFatal";
    case JsErrorWrongRuntime:                  return u"JsErrorWrongRuntime";
    // JsErrorCategoryDiagError
    case JsErrorCategoryDiagError:             return u"JsErrorCategoryDiagError";
    case JsErrorDiagAlreadyInDebugMode:        return u"JsErrorDiagAlreadyInDebugMode";
    case JsErrorDiagNotInDebugMode:            return u"JsErrorDiagNotInDebugMode";
    case JsErrorDiagNotAtBreak:                return u"JsErrorDiagNotAtBreak";
    case JsErrorDiagInvalidHandle:             return u"JsErrorDiagInvalidHandle";
    case JsErrorDiagObjectNotFound:            return u"JsErrorDiagObjectNotFound";
    case JsErrorDiagUnableToPerformAction:     return u"JsErrorDiagUnableToPerformAction";
    default:
        return u"<unknown>";
        break;
    }
}

void Helpers::LogError(__nullterminated const char16_t *msg, ...)
{
    va_list args;
    va_start(args, msg);
    wprintf(u"ERROR: ");
    vfwprintf(stderr, msg, args);
    wprintf(u"\n");
    fflush(stdout);
    va_end(args);
}

int32_t Helpers::LoadBinaryFile(const char * filename, const char *& contents, uint32_t& lengthBytes, bool printFileOpenError)
{
    int32_t hr = S_OK;
    contents = nullptr;
    lengthBytes = 0;
    size_t result;
    FILE * file;

    //
    // Open the file as a binary file to prevent CRT from handling encoding, line-break conversions,
    // etc.
    //
    if (fopen_s(&file, filename, "rb") != 0)
    {
        if (printFileOpenError)
        {
            fprintf(stderr, "Error in opening file '%s' ", filename);
            fprintf(stderr, "\n");
        }
        return E_FAIL;
    }
    // file will not be nullptr if _wfopen_s succeeds
    __analysis_assume(file != nullptr);

    //
    // Determine the file length, in bytes.
    //
    fseek(file, 0, SEEK_END);
    lengthBytes = ftell(file);
    fseek(file, 0, SEEK_SET);
    contents = (const char *)HeapAlloc(GetProcessHeap(), 0, lengthBytes);
    if (nullptr == contents)
    {
        fwprintf(stderr, u"out of memory");
        IfFailGo(E_OUTOFMEMORY);
    }
    //
    // Read the entire content as a binary block.
    //
    result = fread((void*)contents, sizeof(char), lengthBytes, file);
    if (result != lengthBytes)
    {
        fwprintf(stderr, u"Read error");
        IfFailGo(E_FAIL);
    }

Error:
    fclose(file);
    if (contents && FAILED(hr))
    {
        HeapFree(GetProcessHeap(), 0, (void*)contents);
        contents = nullptr;
    }

    return hr;
}

void Helpers::TTReportLastIOErrorAsNeeded(BOOL ok, const char* msg)
{
    if(!ok)
    {
        fprintf(stderr, "Error is: %i %s\n", errno, strerror(errno));
        fprintf(stderr, "Message is: %s\n", msg);

        AssertMsg(false, "IO Error!!!");
        exit(1);
    }
}

//We assume bounded ascii path length for simplicity
#define MAX_TTD_ASCII_PATH_EXT_LENGTH 64

void Helpers::CreateTTDDirectoryAsNeeded(size_t* uriLength, char* uri, const char* asciiDir1, const wchar* asciiDir2)
{
    if(*uriLength + strlen(asciiDir1) + wcslen(asciiDir2) + 2 > MAX_URI_LENGTH || strlen(asciiDir1) >= MAX_TTD_ASCII_PATH_EXT_LENGTH || wcslen(asciiDir2) >= MAX_TTD_ASCII_PATH_EXT_LENGTH)
    {
        wprintf(u"We assume bounded MAX_URI_LENGTH for simplicity.\n");
        wprintf(u"%S, %S, %ls\n", uri, asciiDir1, asciiDir2);
        exit(1);
    }

    int success = 0;
    int extLength = 0;

    extLength = sprintf_s(uri + *uriLength, MAX_TTD_ASCII_PATH_EXT_LENGTH, "%s%s", asciiDir1, TTD_HOST_PATH_SEP);
    if(extLength == -1 || MAX_URI_LENGTH < (*uriLength) + extLength)
    {
        wprintf(u"Failed directory extension 1.\n");
        wprintf(u"%S, %S, %ls\n", uri, asciiDir1, asciiDir2);
        exit(1);
    }
    *uriLength += extLength;

    success = TTDHostMKDir(uri, *uriLength);
    if(success != 0)
    {
        //we may fail because someone else created the directory -- that is ok
        Helpers::TTReportLastIOErrorAsNeeded(errno == EEXIST, "Failed to create directory");
    }

    char realAsciiDir2[MAX_TTD_ASCII_PATH_EXT_LENGTH];
    size_t asciiDir2Length = wcslen(asciiDir2) + 1;
    for(size_t i = 0; i < asciiDir2Length; ++i)
    {
        if(asciiDir2[i] > CHAR_MAX)
        {
            wprintf(u"Test directory names can only include ascii chars.\n");
            exit(1);
        }
        realAsciiDir2[i] = (char)asciiDir2[i];
    }

    extLength = sprintf_s(uri + *uriLength, MAX_TTD_ASCII_PATH_EXT_LENGTH, "%s%s", realAsciiDir2, TTD_HOST_PATH_SEP);
    if(extLength == -1 || MAX_URI_LENGTH < *uriLength + extLength)
    {
        wprintf(u"Failed directory create 2.\n");
        wprintf(u"%S, %S, %ls\n", uri, asciiDir1, asciiDir2);
        exit(1);
    }
    *uriLength += extLength;

    success = TTDHostMKDir(uri, *uriLength);
    if(success != 0)
    {
        //we may fail because someone else created the directory -- that is ok
        Helpers::TTReportLastIOErrorAsNeeded(errno == EEXIST, "Failed to create directory");
    }
}

void Helpers::GetTTDDirectory(const wchar* curi, size_t* uriLength, char* uri, size_t bufferLength)
{
    TTDHostBuildCurrentExeDirectory(uri, uriLength, bufferLength);

    Helpers::CreateTTDDirectoryAsNeeded(uriLength, uri, "_ttdlog", curi);
}

JsTTDStreamHandle CALLBACK Helpers::TTCreateStreamCallback(size_t uriLength, const char* uri, size_t asciiNameLength, const char* asciiName, bool read, bool write)
{
    AssertMsg((read | write) & (!read | !write), "Read/Write streams not supported yet -- defaulting to read only");

    if(uriLength + asciiNameLength + 1 > MAX_URI_LENGTH)
    {
        wprintf(u"We assume bounded MAX_URI_LENGTH for simplicity.");
        exit(1);
    }

    char path[MAX_URI_LENGTH];
    memset(path, 0, MAX_URI_LENGTH);

    memcpy_s(path, MAX_URI_LENGTH, uri, uriLength);
    memcpy_s(path + uriLength, MAX_URI_LENGTH - uriLength, asciiName, asciiNameLength);

    JsTTDStreamHandle res = TTDHostOpen(uriLength + asciiNameLength, path, write);
    if(res == nullptr)
    {
        fprintf(stderr, "Failed to open file: %s\n", path);
    }

    Helpers::TTReportLastIOErrorAsNeeded(res != nullptr, "Failed File Open");
    return res;
}

bool CALLBACK Helpers::TTReadBytesFromStreamCallback(JsTTDStreamHandle handle, byte* buff, size_t size, size_t* readCount)
{
    AssertMsg(handle != nullptr, "Bad file handle.");

    if(size > std::numeric_limits<uint32_t>::max())
    {
        *readCount = 0;
        return false;
    }

    BOOL ok = FALSE;
    *readCount = TTDHostRead(buff, size, (FILE*)handle);
    ok = (*readCount != 0);

    Helpers::TTReportLastIOErrorAsNeeded(ok, "Failed Read!!!");

    return ok ? true : false;
}

bool CALLBACK Helpers::TTWriteBytesToStreamCallback(JsTTDStreamHandle handle, const byte* buff, size_t size, size_t* writtenCount)
{
    AssertMsg(handle != nullptr, "Bad file handle.");

    if(size > std::numeric_limits<uint32_t>::max())
    {
        *writtenCount = 0;
        return false;
    }

    BOOL ok = FALSE;
    *writtenCount = TTDHostWrite(buff, size, (FILE*)handle);
    ok = (*writtenCount == size);

    Helpers::TTReportLastIOErrorAsNeeded(ok, "Failed Read!!!");

    return ok ? true : false;
}

void CALLBACK Helpers::TTFlushAndCloseStreamCallback(JsTTDStreamHandle handle, bool read, bool write)
{
    fflush((FILE*)handle);
    fclose((FILE*)handle);
}

void GetBinaryPathWithFileNameA(char *path, const size_t buffer_size, const char* filename)
{
    char fullpath[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];

    char modulename[_MAX_PATH];
    PlatformAgnostic::SystemInfo::GetBinaryLocation(modulename, _MAX_PATH);
    _splitpath_s(modulename, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
    _makepath_s(fullpath, drive, dir, filename, nullptr);

    size_t len = strlen(fullpath);
    if (len < buffer_size)
    {
        memcpy(path, fullpath, len * sizeof(char));
    }
    else
    {
        len = 0;
    }
    path[len] = char(0);
}
