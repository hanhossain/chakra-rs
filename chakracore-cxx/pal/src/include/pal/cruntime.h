//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    include/pal/cruntime.h

Abstract:

    Header file for C runtime utility functions.



--*/

#ifndef _PAL_CRUNTIME_H_
#define _PAL_CRUNTIME_H_

#include <string.h>
#include <stdarg.h>
#include <pthread.h>

#ifdef __cplusplus

extern "C"
{
#endif // __cplusplus

typedef enum
{
    PFF_NONE  =  0,
    PFF_MINUS =  1,
    PFF_POUND =  2,
    PFF_ZERO  =  4,
    PFF_SPACE =  8,
    PFF_PLUS  = 16
}PRINTF_FORMAT_FLAGS;

typedef enum
{
    WIDTH_DEFAULT = -1,
    WIDTH_STAR    = -2, /* e.g. "%*.10s"  */
    WIDTH_INVALID = -3  /* e.g. "%*3.10s" */
}WIDTH_FLAGS;

typedef enum
{
    PRECISION_DEFAULT = -1,
    PRECISION_STAR    = -2, /* e.g. "%10.*s"  */
    PRECISION_DOT     = -3, /* e.g. "%10.s"   */
    PRECISION_INVALID = -4  /* e.g. "%10.*3s" */
}PRECISION_FLAGS;

typedef enum
{
    PFF_PREFIX_DEFAULT  = -1,
    PFF_PREFIX_SHORT    = 1,
    PFF_PREFIX_LONG     = 2,
    PFF_PREFIX_LONGLONG = 3,
    PFF_PREFIX_LONG_W   = 4
}PRINTF_PREFIXES;

typedef enum
{
    PFF_TYPE_DEFAULT = -1,
    PFF_TYPE_CHAR    = 1,
    PFF_TYPE_STRING  = 2,
    PFF_TYPE_WSTRING = 3,
    PFF_TYPE_INT     = 4,
    PFF_TYPE_P       = 5,
    PFF_TYPE_N       = 6,
    PFF_TYPE_FLOAT   = 7
}PRINTF_TYPES;

typedef enum
{
    SCANF_PREFIX_SHORT = 1,
    SCANF_PREFIX_LONG = 2,
    SCANF_PREFIX_LONGLONG = 3
}SCANF_PREFIXES;

typedef enum
{
    SCANF_TYPE_CHAR     = 1,
    SCANF_TYPE_STRING   = 2,
    SCANF_TYPE_INT      = 3,
    SCANF_TYPE_N        = 4,
    SCANF_TYPE_FLOAT    = 5,
    SCANF_TYPE_BRACKETS = 6,
    SCANF_TYPE_SPACE    = 7
}SCANF_TYPES;

/*******************************************************************************
Function:
  Internal_AddPaddingA

Parameters:
  Out
    - buffer to place padding and given string (In)
  Count
    - maximum chars to be copied so as not to overrun given buffer
  In
    - string to place into (Out) accompanied with padding
  Padding
    - number of padding chars to add
  Flags
    - padding style flags (PRINTF_FORMAT_FLAGS)
*******************************************************************************/
BOOL Internal_AddPaddingA(LPSTR *Out, int32_t Count, LPSTR In, int32_t Padding, int32_t Flags);

/*******************************************************************************
Function:
  PAL_printf_arg_remover

Parameters:
  ap
    - pointer to the va_list from which to remove arguments
  Width
    - the width of the current format option
  Precision
    - the precision of the current format option
  Type
    - the type of the argument for the current format option
  Prefix
    - the prefix for the current format option
*******************************************************************************/
void PAL_printf_arg_remover(va_list *ap, int32_t Width, int32_t Precision, int32_t Type, int32_t Prefix);

/*++
Function:
  Silent_PAL_vsnprintf

See MSDN doc.
--*/
int32_t Silent_PAL_vsnprintf(LPSTR Buffer, int32_t Count, LPCSTR Format, va_list ap);

/*++
Function:
  Silent_PAL_vfprintf

See MSDN doc.
--*/
int Silent_PAL_vfprintf(PAL_FILE *stream, const char *format, va_list ap);



/*++
Function:
  PAL_iswlower

See MSDN

--*/
int PAL_iswlower( char16_t c );


/*++
Function:
  PAL_iswalpha

See MSDN

--*/
int PAL_iswalpha( char16_t c );

#if HAVE_COREFOUNDATION
/*--
Function:
  PAL_iswblank

Returns TRUE if c is a Win32 "blank" character.
--*/
int PAL_iswblank(char16_t c);

/*--
Function:
  PAL_iswcntrl

Returns TRUE if c is a control character.
--*/
int PAL_iswcntrl(char16_t c);

/*--
Function:
  PAL_iswcntrl

Returns TRUE if c is a control character.
--*/
int PAL_iswpunct(char16_t c);
#endif  // HAVE_COREFOUNDATION

/*++

struct PAL_FILE.
Used to mimic the behavior of windows.
fwrite under windows can set the ferror flag,
under BSD fwrite doesn't.
--*/
struct _FILE
{
   FILE *   bsdFilePtr;     /* The BSD file to be passed to the
                            functions needing it. */

   int32_t      PALferrorCode;  /* The ferror code that fwrite sets,
                            incase of error */

   BOOL     bTextMode;     /* Boolean variable to denote that the
                              fle is opened in text/binary mode*/
#if UNGETC_NOT_RETURN_EOF
   BOOL     bWriteOnlyMode;/* Boolean variable to denote that the
                              fle is opened in write-only mode*/ 
#endif //UNGETC_NOT_RETURN_EOF
};

enum CRT_ERROR_CODES
{
    PAL_FILE_NOERROR = 0,
    PAL_FILE_ERROR
};

/* Global variables storing the std streams. Defined in cruntime/file.c. */
extern PAL_FILE PAL_Stdout;
extern PAL_FILE PAL_Stdin; 
extern PAL_FILE PAL_Stderr;

/*++

Functio:

    CRTInitStdStreams.
    
    Initilizes the standard streams.
    Returns TRUE on success, FALSE otherwise.
--*/
BOOL CRTInitStdStreams( void );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _PAL_CRUNTIME_H_ */
