//
// Created by Han Hossain on 3/30/26.
//

#ifndef CHAKRA_RS_PAL_FILE_H
#define CHAKRA_RS_PAL_FILE_H

#include <stdint.h>

typedef int BOOL;
struct __sFILE;
#ifdef __APPLE__
typedef struct __sFILE FILE;
#else
typedef struct _IO_FILE FILE;
#endif

/*++

struct PAL_FILE.
Used to mimic the behavior of windows.
fwrite under windows can set the ferror flag,
under BSD fwrite doesn't.
--*/
typedef struct PAL_FILE
{
    FILE *   bsdFilePtr;     /* The BSD file to be passed to the
                             functions needing it. */

    int32_t      PALferrorCode;  /* The ferror code that fwrite sets,
                             incase of error */

    BOOL     bTextMode;     /* Boolean variable to denote that the
                               fle is opened in text/binary mode*/
#if defined(__linux__)
    BOOL     bWriteOnlyMode;/* Boolean variable to denote that the
                               fle is opened in write-only mode*/
#endif //defined(__linux__)
} PAL_FILE;

#endif //CHAKRA_RS_PAL_FILE_H
