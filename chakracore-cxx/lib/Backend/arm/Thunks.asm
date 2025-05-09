;-------------------------------------------------------------------------------------------------------
; Copyright (C) Microsoft. All rights reserved.
; Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
;-------------------------------------------------------------------------------------------------------
    OPT 2   ; disable listing
#include "ksarm.h"
    OPT 1   ; re-enable listing

    TTL Lib\Backend\arm\Thunks.asm

    ;Js::Var NativeCodeGenerator::CheckCodeGenThunk(Js::RecyclableObject* function, Js::CallInfo callInfo, ...)
    EXPORT  |?CheckCodeGenThunk@NativeCodeGenerator@@SAPAXPAVRecyclableObject@Js@@UCallInfo@3@ZZ|

    ;Js::JavascriptMethod NativeCodeGenerator::CheckCodeGen(Js::JavascriptFunction * function)
    IMPORT  |?CheckCodeGen@NativeCodeGenerator@@SAP6APAXPAVRecyclableObject@Js@@UCallInfo@3@ZZPAVScriptFunction@3@@Z|

    TEXTAREA

;;============================================================================================================
; NativeCodeGenerator::CheckCodeGenThunk
;;============================================================================================================
    ;Js::Var NativeCodeGenerator::CheckCodeGenThunk(Js::RecyclableObject* function, Js::CallInfo callInfo, ...)
    NESTED_ENTRY ?CheckCodeGenThunk@NativeCodeGenerator@@SAPAXPAVRecyclableObject@Js@@UCallInfo@3@ZZ

    PROLOG_PUSH r0-r5,r11,lr      ; save volatile registers and non-volatile registers; r5 is pushed for aligned purposes

    bl   |?CheckCodeGen@NativeCodeGenerator@@SAP6APAXPAVRecyclableObject@Js@@UCallInfo@3@ZZPAVScriptFunction@3@@Z|  ; call  NativeCodeGenerator::CheckCodeGen

    mov     r12, r0               ; back up entryPoint in R12

    EPILOG_POP r0-r5,r11,lr       ; restore arguments and return address

    EPILOG_NOP  bx   r12          ; jump (tail call) to new entryPoint

    NESTED_END

;;============================================================================================================
    END
