//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
// (hintCode, isNotOptimized, hintlevel, description, consequences, suggestion)

PERFHINT_REASON(HasTryBlock,                        true, PerfHintLevels::L1,      u"Function has try block",                 u"Un-optimized JIT code generated for this function", u"Move perf sensitive block inside of try to different function")
PERFHINT_REASON(HasTryBlock_Verbose,                true, PerfHintLevels::VERBOSE, u"Function has try block",                 u"Un-optimized JIT code generated for this function", u"Move perf sensitive block inside of try to different function")
PERFHINT_REASON(SrcIsEval,                          true, PerfHintLevels::L1,      u"Source is inside eval statement",        u"Extra scopes, affect inlining, high overhead in the JIT code", u"Check usage of eval statement")
PERFHINT_REASON(CallsEval,                          true, PerfHintLevels::L1,      u"Function calls eval statement",          u"Extra scopes, affect inlining, high overhead in the JIT code", u"Check usage of eval statement")
PERFHINT_REASON(CallsEval_Verbose,                  true, PerfHintLevels::VERBOSE, u"Function calls eval statement",          u"Extra scopes, affect inlining, high overhead in the JIT code", u"Check usage of eval statement")
PERFHINT_REASON(ChildCallsEval,                     true, PerfHintLevels::VERBOSE, u"Function's child calls eval statement",  u"Extra scopes, affect inlining, high overhead in the JIT code", u"Check usage of eval statement")
PERFHINT_REASON(HasWithBlock,                       true, PerfHintLevels::L1,      u"Function has with statement",            u"Slower lookups, high overhead in the JIT code", u"Avoid using with statement")
PERFHINT_REASON(HeapArgumentsDueToWriteToFormals,   true, PerfHintLevels::L1,      u"Arguments object not optimized due to write to formals", u"Mapping between formals and arguments object cannot be maintained", u"Check the usage of write to formals in the function")
PERFHINT_REASON(HeapArgumentsDueToNonLocalRef,      true, PerfHintLevels::L1,      u"Arguments object not optimized as there were some nested functions or non-local refs found in nested functions",  u"Scope object creation is required in this case", u"Check for nested functions and non-local refs inside"  )
PERFHINT_REASON(HeapArgumentsModification,          true, PerfHintLevels::L1,      u"Modification to arguments",              u"Slower lookups, high overhead in the JIT code", u"Avoid modification to the arguments")
PERFHINT_REASON(HeapArgumentsCreated,               true, PerfHintLevels::L1,      u"Arguments object not optimized",         u"Slower lookups, high overhead in the JIT code", u"Check the usage of arguments in the function")
PERFHINT_REASON(PolymorphicInilineCap,              true, PerfHintLevels::L1,      u"Function has reached polymorphic-inline cap", u"This function will not inline more than 4 functions for this call-site.", u"Check the polymorphic usage of this function")
