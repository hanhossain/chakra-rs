//-------------------------------------------------------------------------------------------------------
// ChakraCore/Pal
// Contains portions (c) copyright Microsoft, portions copyright (c) the .NET Foundation and Contributors
// and edits (c) copyright the ChakraCore Contributors.
// See THIRD-PARTY-NOTICES.txt in the project root for .NET Foundation license
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

/*++



Module Name:

    include/pal/utils.h

Abstract:
    Miscellaneous helper functions for the PAL, which don't fit anywhere else



--*/

#ifndef _PAL_UTILS_H_
#define _PAL_UTILS_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pal_assert.h>

// Alignment helpers (copied for PAL use from stdmacros.h)

inline size_t ALIGN_UP(size_t val, size_t alignment)
{
    // alignment must be a power of 2 for this implementation to work (need modulo otherwise)
    _ASSERTE(0 == (alignment & (alignment - 1)));
    size_t result = (val + (alignment - 1)) & ~(alignment - 1);
    _ASSERTE(result >= val);      // check for overflow
    return result;
}

inline void* ALIGN_UP(void* val, size_t alignment)
{
    return (void*)ALIGN_UP((size_t)val, alignment);
}

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*++
Function:
  UTIL_inverse_wcspbrk

  Opposite of wcspbrk : searches a string for the first character NOT in the 
  given set

Parameters :
    LPWSTR lpwstr :   string to search
    LPCWSTR charset : list of characters to search for
                                      
Return value :
    pointer to first character of lpwstr that isn't in the set
    NULL if all characters are in the set                                                                 
--*/
LPWSTR UTIL_inverse_wcspbrk(LPWSTR lpwstr, LPCWSTR charset);

/*++
Function : 
    UTIL_IsReadOnlyBitsSet
    
    Takes a struct stat *
    Returns true if the file is read only,
--*/
BOOL UTIL_IsReadOnlyBitsSet( struct stat * stat_data );

/*++
Function : 
    UTIL_IsExecuteBitsSet
    
    Takes a struct stat *
    Returns true if the file is executable.
--*/
BOOL UTIL_IsExecuteBitsSet( struct stat * stat_data );


/*++
Function : 
    UTIL_WCToMB_Alloc
    
    Converts a wide string to a multibyte string, allocating the required buffer
    
Parameters :
    LPCWSTR lpWideCharStr : string to convert
    int cchWideChar : number of wide characters to convert
                      (-1 to convert a complete null-termnated string)
    
Return Value :
    newly allocated buffer containing the converted string. Conversion is 
    performed using CP_ACP. Buffer is allocated with malloc(), release it 
    with free().
    In case if failure, LastError will be set.
--*/
LPSTR UTIL_WCToMB_Alloc(LPCWSTR lpWideCharStr, int cchWideChar);

/*++
Function : 
    UTIL_MBToWC_Alloc
    
    Converts a multibyte string to a wide string, allocating the required buffer
    
Parameters :
    LPCSTR lpMultiByteStr : string to convert
    int cbMultiByte : number of bytes to convert 
                      (-1 to convert a complete null-termnated string)
    
Return Value :
    newly allocated buffer containing the converted string. Conversion is 
    performed using CP_ACP. Buffer is allocated with malloc(), release it 
    with free().
    In case if failure, LastError will be set.
--*/
LPWSTR UTIL_MBToWC_Alloc(LPCSTR lpMultiByteStr, int cbMultiByte);

#if HAVE_VM_ALLOCATE
#include <mach/kern_return.h>

/*++
Function:
  UTIL_MachErrorToPalError

    Maps a Mach kern_return_t to a Win32 error code.
--*/
uint32_t UTIL_MachErrorToPalError(kern_return_t MachReturn);

/*++
Function:
  UTIL_SetLastErrorFromMach

    Sets Win32 LastError according to the argument Mach kern_return_t value,
    provided it indicates an error.  If the argument indicates success, does
    not modify LastError.
--*/
void UTIL_SetLastErrorFromMach(kern_return_t MachReturn);

#endif //HAVE_VM_ALLOCATE

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _PAL_UTILS_H_ */
