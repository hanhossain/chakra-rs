//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/***
*input.c - C formatted input, used by scanf, etc.
*

*
*Purpose:
*       defines _input() to do formatted input; called from scanf(),
*       etc. functions.  This module defines _cscanf() instead when
*       CPRFLAG is defined.  The file cscanf.c defines that symbol
*       and then includes this file in order to implement _cscanf().
*
*Note:
*       this file is included in safecrt.lib build directly, please refer
*       to safecrt_[w]input_s.c
*
*******************************************************************************/


#define ALLOW_RANGE /* enable "%[a-z]"-style scansets */


/* temporary work-around for compiler without 64-bit support */

#ifndef _INTEGRAL_MAX_BITS
#define _INTEGRAL_MAX_BITS  64
#endif  /* _INTEGRAL_MAX_BITS */

// typedef __int64_t __int64;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define UNALIGNED

#define _BEGIN_SECURE_CRT_DEPRECATION_DISABLE
#define _END_SECURE_CRT_DEPRECATION_DISABLE

#define _CVTBUFSIZE (309+40) /* # of digits in max. dp value + slop */

//#include <cruntime.h>
//#include <stdio.h>
//#include <ctype.h>
//#include <cvt.h>
//#include <conio.h>
//#include <stdarg.h>
//#include <string.h>
//#include <internal.h>
//#include <fltintrn.h>
//#include <malloc.h>
//#include <locale.h>
//#include <mtdll.h>
//#include <stdlib.h>
//#include <setlocal.h>
//#include <dbgint.h>

//#ifndef _INC_INTERNAL_SAFECRT
//#include <internal_securecrt.h>
//#endif  /* _INC_INTERNAL_SAFECRT */

//#ifdef _MBCS
//#undef _MBCS
//#endif  /* _MBCS */
//#include <tchar.h>

#define _MBTOWC(x,y,z) _minimal_chartowchar( x, y )

#ifdef UNICODE
#define _istspace(x)    isspace( ( char )( x & 0x00FF ) )
#else
#define _istspace(x)    isspace(x)
#endif

#define _malloc_crt PAL_malloc
#define _realloc_crt PAL_realloc
#define _free_crt PAL_free

#define _FASSIGN(flag, argument, number, dec_point, locale) _safecrt_fassign((flag), (argument), (number))
#define _WFASSIGN(flag, argument, number, dec_point, locale) _safecrt_wfassign((flag), (argument), (number))

#if defined (UNICODE)
#define ALLOC_TABLE 1
#else  /* defined (UNICODE) */
#define ALLOC_TABLE 0
#endif  /* defined (UNICODE) */

#define HEXTODEC(chr)   _hextodec(chr)

#define LEFT_BRACKET    ('[' | ('a' - 'A')) /* 'lowercase' version */

static int _hextodec(_TCHAR);
#ifdef CPRFLAG

#define INC()           (++charcount, _inc())
#define UN_INC(chr)     (--charcount, _un_inc(chr))
#define EAT_WHITE()     _whiteout(&charcount)

static int _inc(void);
static void _un_inc(int);
static int _whiteout(int *);

#else  /* CPRFLAG */

#define INC()           (++charcount, _inc(stream))
#define UN_INC(chr)     (--charcount, _un_inc(chr, stream))
#define EAT_WHITE()     _whiteout(&charcount, stream)

static int _inc(miniFILE *);
static void _un_inc(int, miniFILE *);
static int _whiteout(int *, miniFILE *);

#endif  /* CPRFLAG */

#ifndef _UNICODE
#define _ISDIGIT(chr)   isdigit((unsigned char)chr)
#define _ISXDIGIT(chr)  isxdigit((unsigned char)chr)
#else  /* _UNICODE */
#define _ISDIGIT(chr)   ( !(chr & 0xff00) && isdigit( ((chr) & 0x00ff) ) )
#define _ISXDIGIT(chr)  ( !(chr & 0xff00) && isxdigit( ((chr) & 0x00ff) ) )
#endif  /* _UNICODE */

#define MUL10(x)        ( (((x)<<2) + (x))<<1 )


#define LONGLONG_IS_INT64 1     /* 1 means long long is same as int64
                                   0 means long long is same as long */

/***
*  int __check_float_string(size_t,size_t *, _TCHAR**, _TCHAR*, int*)
*
*  Purpose:
*       Check if there is enough space insert onemore character in the given
*       block, if not then allocate more memory.
*
*  Return:
*       FALSE if more memory needed and the reallocation failed.
*
*******************************************************************************/

static int __check_float_string(size_t nFloatStrUsed,
                                size_t *pnFloatStrSz,
                                _TCHAR **pFloatStr,
                                _TCHAR *floatstring,
                                int *pmalloc_FloatStrFlag)
{
    void *tmpPointer;
    _ASSERTE(nFloatStrUsed<=(*pnFloatStrSz));
    if (nFloatStrUsed==(*pnFloatStrSz))
    {
        size_t newSize;

        // Will (*pnFloatStrSz) * 2 * sizeof(_TCHAR) overflow?
        if ( *pnFloatStrSz > (SIZE_MAX / 2 / sizeof(_TCHAR)))
        {
            return FALSE;
        }

        newSize = *pnFloatStrSz * 2 * sizeof(_TCHAR);
        
        if ((*pFloatStr)==floatstring)
        {
            if (((*pFloatStr)=(_TCHAR *)_malloc_crt(newSize))==NULL)
            {
                return FALSE;
            }

            (*pmalloc_FloatStrFlag)=1;

            memcpy((*pFloatStr),floatstring,(*pnFloatStrSz)*sizeof(_TCHAR));
            (*pnFloatStrSz)*=2;
        }
        else
        {
            if ((tmpPointer=(_TCHAR *)_realloc_crt((*pFloatStr), newSize))==NULL)
            {
                return FALSE;
            }
            (*pFloatStr)=(_TCHAR *)(tmpPointer);
            (*pnFloatStrSz)*=2;
        }
    }
    return TRUE;
}


#define ASCII       32           /* # of bytes needed to hold 256 bits */

#define SCAN_SHORT     0         /* also for float */
#define SCAN_LONG      1         /* also for double */
#define SCAN_L_DOUBLE  2         /* only for int32_t double */

#define SCAN_NEAR    0
#define SCAN_FAR     1

#ifndef _UNICODE
#define TABLESIZE    ASCII
#else  /* _UNICODE */
#define TABLESIZE    (ASCII * 256)
#endif  /* _UNICODE */


/***
*int _input(stream, format, arglist), static int input(format, arglist)
*
*Purpose:
*   get input items (data items or literal matches) from the input stream
*   and assign them if appropriate to the items thru the arglist. this
*   function is intended for internal library use only, not for the user
*
*   The _input entry point is for the normal scanf() functions
*   The input entry point is used when compiling for _cscanf() [CPRFLAF
*   defined] and is a static function called only by _cscanf() -- reads from
*   console.
*
*   This code also defines _input_s, which works differently for %c, %s & %[.
*   For these, _input_s first picks up the next argument from the variable
*   argument list & uses it as the maximum size of the character array pointed
*   to by the next argument in the list.
*
*Entry:
*   FILE *stream - file to read from
*   char *format - format string to determine the data to read
*   arglist - list of pointer to data items
*
*Exit:
*   returns number of items assigned and fills in data items
*   returns EOF if error or EOF found on stream before 1st data item matched
*
*Exceptions:
*
*******************************************************************************/

    #define _INTRN_LOCALE_CONV( x ) localeconv()

/* _hextodec() returns a value of 0-15 and expects a char 0-9, a-f, A-F */
/* _inc() is the one place where we put the actual getc code. */
/* _whiteout() returns the first non-blank character, as defined by isspace() */

static int _hextodec ( _TCHAR chr)
{
    return _ISDIGIT(chr) ? chr : (chr & ~(_T('a') - _T('A'))) - _T('A') + 10 + _T('0');
}

#ifdef CPRFLAG

static int _inc(void)
{
    return (_gettche_nolock());
}

static void _un_inc(int chr)
{
    if (_TEOF != chr) {
        _ungettch_nolock(chr);
    }
}

static int _whiteout(REG1 int* counter)
{
    REG2 int ch;

    do
    {
        ++*counter;
        ch = _inc();

        if (ch == _TEOF)
        {
            break;
        }
    }
    while(_istspace((_TUCHAR)ch));
    return ch;
}

#else  /* CPRFLAG */

static int _inc(miniFILE* fileptr)
{
    return (_gettc_nolock(fileptr));
}

static void _un_inc(int chr, miniFILE* fileptr)
{
    if (_TEOF != chr) {
        _ungettc_nolock(chr,fileptr);
    }
}

static int _whiteout(int* counter, miniFILE* fileptr)
{
    int ch;

    do
    {
        ++*counter;
        ch = _inc(fileptr);

        if (ch == _TEOF)
        {
            break;
        }
    }
    while(_istspace((_TUCHAR)ch));
    return ch;
}

#endif  /* CPRFLAG */

