//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include <string>
#include "CommonCorePch.h"
#include "Memory/PageHeapBlockTypeFilter.h"
#include "Core/ConfigFlagsTable.h"

#include <chakracore-sys/src/str_helper.rs.h>
#include <rust/cxx.h>

#undef DebugBreak

// Initialization order
//  AB AutoSystemInfo
//  AD PerfCounter
//  AE PerfCounterSet
//  AM Output/Configuration
//  AN MemProtectHeap
//  AP DbgHelpSymbolManager
//  AQ CFGLogger
//  AR LeakReport
//  AS JavascriptDispatch/RecyclerObjectDumper
//  AT HeapAllocator/RecyclerHeuristic
//  AU RecyclerWriteBarrierManager
#pragma warning(disable:4075)       // initializers put in unrecognized initialization area on purpose
#pragma init_seg(".CRT$XCAM")
namespace Js
{
    NumberSet::NumberSet() : set(&NoCheckHeapAllocator::Instance) {}

    void NumberSet::Add(uint32_t x)
    {
        set.Item(x);
    }

    bool NumberSet::Contains(uint32_t x)
    {
        return set.Contains(x);
    }


    NumberPairSet::NumberPairSet() : set(&NoCheckHeapAllocator::Instance) {}

    void NumberPairSet::Add(uint32_t x, uint32_t y)
    {
        set.Item(NumberPair(x, y));
    }

    bool NumberPairSet::Contains(uint32_t x, uint32_t y)
    {
        return set.Contains(NumberPair(x, y));
    }

    NumberTrioSet::NumberTrioSet() : set(&NoCheckHeapAllocator::Instance) {}

    void NumberTrioSet::Add(uint32_t x, uint32_t y, uint32_t z)
    {
        set.Item(NumberTrio(x, y, z));
    }

    bool NumberTrioSet::Contains(uint32_t x, uint32_t y, uint32_t z)
    {
        return set.Contains(NumberTrio(x, y, z));
    }

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class String
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------

    String::String()
    {
        this->pszValue = NULL;
    }

    String::String(__in_z_opt const char16_t* psz)
    {
        this->pszValue = NULL;
        Set(psz);
    }

    String::~String()
    {
        if(NULL != this->pszValue)
        {
            NoCheckHeapDeleteArray(std::u16string(this->pszValue).length() + 1, this->pszValue);
        }
    }

    ///----------------------------------------------------------------------------
    ///
    /// String::Set
    ///
    /// Frees the existing string if any
    /// allocates a new buffer to copy the new string
    ///
    ///----------------------------------------------------------------------------

    void
    String::Set(__in_z_opt const char16_t* pszValue)
    {
        if(NULL != this->pszValue)
        {
            NoCheckHeapDeleteArray(std::u16string(this->pszValue).length() + 1, this->pszValue);
        }

        if(NULL != pszValue)
        {
            size_t size    = 1 + std::u16string(pszValue).length();
            this->pszValue  = NoCheckHeapNewArray(char16_t, size);
            wcscpy_s(this->pszValue, size, pszValue);
        }
        else
        {
            this->pszValue = NULL;
        }
    }

    template <>
    bool RangeUnitContains<SourceFunctionNode>(RangeUnit<SourceFunctionNode> unit, SourceFunctionNode n)
    {
        Assert(n.functionId != (uint32_t)-1);

        if ((n.sourceContextId >= unit.i.sourceContextId) &&
            (n.sourceContextId <= unit.j.sourceContextId)
            )
        {
            if ((n.sourceContextId == unit.j.sourceContextId && -2 == unit.j.functionId) ||  //#.#-#.* case
                (n.sourceContextId == unit.i.sourceContextId && -2 == unit.i.functionId)     //#.*-#.# case
                )
            {
                return true;
            }

            if ((n.sourceContextId == unit.j.sourceContextId && -1 == unit.j.functionId) || //#.#-#.+ case
                (n.sourceContextId == unit.i.sourceContextId && -1 == unit.i.functionId)     //#.+-#.# case
                )
            {
                return n.functionId != 0;
            }

            if ((n.sourceContextId == unit.i.sourceContextId && n.functionId < unit.i.functionId) || //excludes all values less than functionId LHS
                (n.sourceContextId == unit.j.sourceContextId && n.functionId > unit.j.functionId)) ////excludes all values greater than functionId RHS
            {
                return false;
            }

            return true;
        }

        return false;
    }

    template <>
    Js::RangeUnit<Js::SourceFunctionNode> GetFullRange()
    {
        RangeUnit<SourceFunctionNode> unit;
        unit.i.sourceContextId = 0;
        unit.j.sourceContextId = UINT_MAX;
        unit.i.functionId = 0;
        unit.j.functionId = (uint)-3;
        return unit;
    }

    template <>
    SourceFunctionNode GetPrevious(SourceFunctionNode unit)
    {
        SourceFunctionNode prevUnit = unit;
        prevUnit.functionId--;
        if (prevUnit.functionId == UINT_MAX)
        {
            prevUnit.sourceContextId--;
        }
        return prevUnit;
    }

    template <>
    SourceFunctionNode GetNext(SourceFunctionNode unit)
    {
        SourceFunctionNode nextUnit = unit;
        nextUnit.functionId++;
        if (nextUnit.functionId == 0)
        {
            nextUnit.sourceContextId++;
        }
        return nextUnit;
    }

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class Phases
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------

    bool
    Phases::IsEnabled(Phase phase)
    {
        return this->phaseList[(int)phase].valid;
    }

    bool
    Phases::IsEnabled(Phase phase, uint sourceContextId, Js::LocalFunctionId functionId)
    {
        return  this->phaseList[(int)phase].valid &&
                this->phaseList[(int)phase].range.InRange(SourceFunctionNode(sourceContextId, functionId));
    }

    bool
    Phases::IsEnabledForAll(Phase phase)
    {
        return  this->phaseList[(int)phase].valid &&
                this->phaseList[(int)phase].range.ContainsAll();
    }

    Range *
    Phases::GetRange(Phase phase)
    {
        return &this->phaseList[(int)phase].range;
    }

    void
    Phases::Enable(Phase phase)
    {
        this->phaseList[(int)phase].valid = true;
    }

    void
    Phases::Disable(Phase phase)
    {
        this->phaseList[(int)phase].valid = false;
        this->phaseList[(int)phase].range.Clear();
    }

    Phase
    Phases::GetFirstPhase()
    {
        int i= -1;
        while(!this->phaseList[++i].valid)
        {
            if(i >= PhaseCount - 1)
            {
                return InvalidPhase;
            }
        }
        return Phase(i);
    }


    //
    // List of names of all the flags
    //

    const char16_t* const FlagNames[FlagCount + 1] =
    {
    #define FLAG(type, name, ...) _u(#name),
    #include "Interface/ConfigFlagsList.h"
        NULL
    #undef FLAG
    };


    //
    // List of names of all the Phases
    //

    const char16_t* const PhaseNames[PhaseCount + 1] =
    {
    u"All",
        u"BGJit",
        u"Module",
        u"LibInit",
            u"JsLibInit",
        u"Parse",
            u"RegexCompile",
            u"DeferParse",
            u"Redeferral",
            u"DeferEventHandlers",
            u"FunctionSourceInfoParse",
            u"StringTemplateParse",
            u"CreateParserState",
            u"SkipNestedDeferred",
            u"CacheScopeInfoNames",
            u"ScanAhead",
            u"ParallelParse",
            u"EarlyReferenceErrors",
            u"EarlyErrorOnAssignToCall",
            u"BgParse",
        u"ByteCode",
            u"CachedScope",
            u"StackFunc",
            u"StackClosure",
            u"DisableStackFuncOnDeferredEscape",
            u"DelayCapture",
            u"DebuggerScope",
            u"ByteCodeSerialization",
                u"VariableIntEncoding",
            u"NativeCodeSerialization",
            u"OptimizeBlockScope",
        u"Delay",
            u"Speculation",
            u"GatherCodeGenData",
        u"Wasm",
            // Wasm frontend
            u"WasmBytecode", // Supports -off,-dump,-trace,-profile
            u"WasmReader", // Support -trace,-profile
            u"WasmSection", // Supports -trace
            u"WasmOpCodeDistribution", // Support -dump
            // Wasm features per functions
            u"WasmDeferred",
            u"WasmValidatePrejit",
            u"WasmInOut", // Trace input and output of wasm calls
            u"WasmMemWrites", // Trace memory writes
        u"Asmjs",
            u"AsmjsTmpRegisterAllocation",
            u"AsmjsEncoder",
            u"AsmjsInterpreter",
            u"AsmJsJITTemplate",
            u"AsmjsFunctionEntry",
            u"AsmjsInterpreterStack",
            u"AsmjsEntryPointInfo",
            u"AsmjsCallDebugBreak",
        u"BackEnd",
            u"IRBuilder",
                u"SwitchOpt",
                u"BailOnNoProfile",
                u"BackendConcatExprOpt",
                u"ClosureRangeCheck",
                u"ClosureRegCheck",
            u"Inline",
                u"InlineRecursive",
                u"InlineAtEveryCaller",      //Inlines a function, say, foo at every caller of foo. Doesn't guarantee all the calls within foo are inlined too.
                u"InlineTree",               //Inlines every function within a top function, say, foo (which needs to be top function) Note: -force:inline achieves the effect of both -force:InlineTree & -force:InlineAtEveryCaller
                u"TryAggressiveInlining",
                u"InlineConstructors",
                u"InlineBuiltIn",
                u"InlineInJitLoopBody",
                u"InlineAccessors",
                u"InlineGetters",
                u"InlineSetters",
                u"InlineApply",
                u"InlineApplyTarget",
                u"InlineApplyWithoutArrayArg",
                u"InlineAnyCallApplyTarget",
                u"BailOutOnNotStackArgs",
                u"InlineCall",
                u"InlineCallTarget",
                u"PartialPolymorphicInline",
                u"PolymorphicInline",
                u"PolymorphicInlineFixedMethods",
                u"InlineOutsideLoops",
                u"InlineFunctionsWithLoops",
                u"EliminateArgoutForInlinee",
                u"InlineBuiltInCaller",
                u"InlineArgsOpt",
                    u"RemoveInlineFrame",
                u"InlinerConstFold",
                u"InlineCallbacks",
        u"ExecBOIFastPath",
            u"FGBuild",
                u"OptimizeTryFinally",
                u"RemoveBreakBlock",
                u"TailDup",
            u"FGPeeps",
            u"GlobOpt",
                u"PathDepBranchFolding",
                u"OptimizeTryCatch",
                u"CaptureByteCodeRegUse",
                u"Backward",
                    u"TrackIntUsage",
                    u"TrackNegativeZero",
                    u"TypedArrayVirtual",
                    u"TrackIntOverflow",
                    u"TrackCompoundedIntOverflow",
                u"Forward",
                    u"ValueTable",
                    u"ValueNumbering",
                    u"AVTInPrePass",
                    u"PathDependentValues",
                        u"TrackRelativeIntBounds",
                            u"BoundCheckElimination",
                                u"BoundCheckHoist",
                                    u"LoopCountBasedBoundCheckHoist",
                    u"CopyProp",
                        u"ObjPtrCopyProp",
                    u"ConstProp",
                    u"Int64ConstProp",
                    u"ConstFold",
                    u"CSE",
                    u"HoistConstInt",
                    u"TypeSpec",
                    u"AggressiveIntTypeSpec",
                    u"AggressiveMulIntTypeSpec",
                    u"LossyIntTypeSpec",
                    u"FloatTypeSpec",
                    u"StringTypeSpec",
                    u"InductionVars",
                    u"Invariants",
                    u"FieldCopyProp",
                    u"FieldPRE",
                    u"MakeObjSymLiveInLandingPad",
                    u"HostOpt",
                    u"ObjTypeSpec",
                        u"ObjTypeSpecNewObj",
                        u"ObjTypeSpecIsolatedFldOps",
                        u"ObjTypeSpecIsolatedFldOpsWithBailOut",
                        u"ObjTypeSpecStore",
                        u"EquivObjTypeSpec",
                        u"EquivObjTypeSpecByDefault",
                        u"TraceObjTypeSpecTypeGuards",
                        u"TraceObjTypeSpecWriteGuards",
                        u"LiveOutFields",
                        u"DisabledObjTypeSpec",
                        u"DepolymorphizeInlinees",
                        u"ReuseAuxSlotPtr",
                        u"PolyEquivTypeGuard",
                        u"DeadStoreTypeChecksOnStores",
                        #if DBG
                            u"SimulatePolyCacheWithOneTypeForFunction",
                        #endif
                    u"CheckThis",
                    u"StackArgOpt",
                    u"StackArgFormalsOpt",
                    u"StackArgLenConstOpt",
                    u"IndirCopyProp",
                    u"ArrayCheckHoist",
                        u"ArrayMissingValueCheckHoist",
                        u"ArraySegmentHoist",
                            u"JsArraySegmentHoist",
                        u"ArrayLengthHoist",
                        u"EliminateArrayAccessHelperCall",
                    u"NativeArray",
                        u"NativeNewScArray",
                        u"NativeArrayConversion",
                        u"CopyOnAccessArray",
                        u"NativeArrayLeafSegment",
                    u"TypedArrayTypeSpec",
                    u"LdLenIntSpec",
                    u"FixDataProps",
                    u"FixMethodProps",
                    u"FixAccessorProps",
                    u"FixDataVarProps",
                    u"UseFixedDataProps",
                    u"UseFixedDataPropsInInliner",
                    u"LazyBailout",
                        u"LazyBailoutOnImplicitCalls",
                        u"LazyFixedDataBailout",
                        u"LazyFixedTypeBailout",
                    u"FixedMethods",
                        u"FEFixedMethods",
                        u"FixedFieldGuardCheck",
                        u"FixedNewObj",
                            u"JitAllocNewObj",
                        u"FixedCtorInlining",
                        u"FixedCtorCalls",
                        u"FixedScriptMethodInlining",
                        u"FixedScriptMethodCalls",
                        u"FixedBuiltInMethodInlining",
                        u"FixedBuiltInMethodCalls",
                        u"SplitNewScObject",
                    u"OptTagChecks",
                    u"MemOp",
                        u"MemSet",
                        u"MemCopy",
                    u"IncrementalBailout",
                u"DeadStore",
                    u"ReverseCopyProp",
                    u"MarkTemp",
                        u"MarkTempNumber",
                        u"MarkTempObject",
                        u"MarkTempNumberOnTempObject",
                    u"SpeculationPropagationAnalysis",
            u"DumpGlobOptInstr", // Print the Globopt instr string in post lower dumps
            u"Lowerer",
                u"FastPath",
                    u"LoopFastPath",
                    u"MathFastPath",
                    u"Atom",
                        u"MulStrengthReduction",
                        u"AgenPeeps",
                    u"BranchFastPath",
                    u"CallFastPath",
                    u"BitopsFastPath",
                    u"OtherFastPath",
                    u"ObjectFastPath",
                    u"ProfileBasedFldFastPath",
                    u"AddFldFastPath",
                    u"RootObjectFldFastPath",
                    u"ArrayLiteralFastPath",
                    u"ArrayCtorFastPath",
                    u"NewScopeSlotFastPath",
                    u"FrameDisplayFastPath",
                    u"HoistMarkTempInit",
                    u"HoistConstAddr",
                u"JitWriteBarrier",
                u"PreLowererPeeps",
                u"CFGInJit",
                u"TypedArray",
                u"TracePinnedTypes",
            u"InterruptProbe",
            u"EncodeConstants",
            u"RegAlloc",
                u"Liveness",
                    u"RegParams",
                u"LinearScan",
                    u"OpHelperRegOpt",
                    u"StackPack",
                    u"SecondChance",
                    u"RegionUseCount",
                    u"RegHoistLoads",
                    u"ClearRegLoopExit",
            u"Peeps",
            u"Layout",
            u"EHBailoutPatchUp",
            u"FinalLower",
            u"PrologEpilog",
            u"InsertNOPs",
            u"Encoder",
                u"Assembly",
                u"Emitter",
                u"DebugBreak",
    #if defined(_M_X64)
                u"BrShorten",
                    u"LoopAlign",
    #endif

    #ifdef RECYCLER_WRITE_BARRIER
    #if DBG_DUMP
        u"SWB",
    #endif
    #endif
        u"Run",
            u"Interpreter",
            u"EvalCompile",
                u"FastIndirectEval",
            u"IdleDecommit",
            u"IdleCollect",
            u"Marshal",
            u"MemoryAllocation",
    #ifdef RECYCLER_PAGE_HEAP
                u"PageHeap",
    #endif
                u"LargeMemoryAllocation",
                u"PageAllocatorAlloc",
            u"Recycler",
                u"ThreadCollect",
                u"ExplicitFree",
                u"ExpirableCollect",
                u"GarbageCollect",
                u"ConcurrentCollect",
                    u"BackgroundResetMarks",
                    u"BackgroundFindRoots",
                    u"BackgroundRescan",
                    u"BackgroundRepeatMark",
                    u"BackgroundFinishMark",
                u"ConcurrentPartialCollect",
                u"ParallelMark",
                u"PartialCollect",
                    u"ResetMarks",
                    u"ResetWriteWatch",
                    u"FindRoot",
                        u"FindRootArena",
                        u"FindImplicitRoot",
                        u"FindRootExt",
                    u"ScanStack",
                    u"ConcurrentMark",
                    u"ConcurrentWait",
                    u"Rescan",
                    u"Mark",
                    u"Sweep",
                        u"SweepWeak",
                        u"SweepSmall",
                        u"SweepLarge",
                        u"SweepPartialReuse",
                    u"ConcurrentSweep",
                    u"Finalize",
                    u"Dispose",
                    u"FinishPartial",
            u"Host",
            u"BailOut",
            u"BailIn",
            u"GeneratorGlobOpt",
            u"RegexQc",
            u"RegexOptBT",
            u"InlineCache",
            u"PolymorphicInlineCache",
            u"MissingPropertyCache",
            u"PropertyCache", // Trace caching of property lookups using PropertyString and JavascriptSymbol
            u"CloneCacheInCollision",
            u"ConstructorCache",
            u"InlineCandidate",
            u"ScriptFunctionWithInlineCache",
            u"IsConcatSpreadableCache",
            u"Arena",
            u"ApplyUsage",
            u"ObjectHeaderInlining",
                u"ObjectHeaderInliningForConstructors",
                u"ObjectHeaderInliningForObjectLiterals",
                u"ObjectHeaderInliningForEmptyObjects",
            u"OptUnknownElementName",
            u"TypePropertyCache",
    #if DBG_DUMP
            u"InlineSlots",
    #endif
            u"DynamicProfile",
    #ifdef DYNAMIC_PROFILE_STORAGE
            u"DynamicProfileStorage",
    #endif
            u"JITLoopBody",
            u"JITLoopBodyInTryCatch",
            u"JITLoopBodyInTryFinally",
            u"ReJIT",
            u"ExecutionMode",
            u"SimpleJitDynamicProfile",
            u"SimpleJit",
            u"FullJit",
            u"FailNativeCodeInstall",
            u"PixelArray",
            u"Etw",
            u"Profiler",
            u"CustomHeap",
            u"XDataAllocator",
            u"PageAllocator",
            u"StringConcat",
    #if DBG_DUMP
            u"PRNG",
    #endif
            u"PreReservedHeapAlloc",
            u"CFG",
            u"ExceptionStackTrace",
            u"ExtendedExceptionInfoStackTrace",
            u"TypeHandlerTransition",
            u"Debugger",
                u"ENC",
            u"ConsoleScope",
            u"ScriptProfiler",
            u"JSON",
            u"Intl",
            u"RegexResultNotUsed",
            u"Error",
            u"PropertyRecord",
            u"TypePathDynamicSize",
            u"ObjectCopy",
            u"ShareTypesWithAttributes",
            u"ShareAccessorTypes",
            u"ShareFuncTypes",
            u"ShareCrossSiteFuncTypes",
            u"ConditionalCompilation",
            u"InterpreterProfile",
            u"InterpreterAutoProfile",
            u"ByteCodeConcatExprOpt",
            u"TraceInlineCacheInvalidation",
            u"TracePropertyGuards",
            u"PerfHint",
            u"TypeShareForChangePrototype",
            u"DeferSourceLoad",
            u"DataCache",
            u"ObjectMutationBreakpoint",
            u"NativeCodeData",
            u"XData",
        nullptr
    };


    //
    // Description of flags
    //
    const char16_t* const FlagDescriptions[FlagCount + 1] =
    {
    #define FLAG(type, name, description, ...) _u(description),
    #include "Interface/ConfigFlagsList.h"
        NULL
    #undef FLAG
    };

    //
    // Parent flag categorization of flags
    //
    const Flag FlagParents[FlagCount + 1] =
    {
    #define FLAG(type, name, description, defaultValue, parentName, ...) parentName##Flag,
    #include "Interface/ConfigFlagsList.h"
        InvalidFlag
    #undef FLAG
    };

    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class ConfigFlagsTable
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::ConfigFlagsTable
    ///
    /// Constructor initializes all the flags with their default values. The nDummy
    /// variable is used to prevent the compiler error due to the trailing comma
    /// when we generate the list of flags.
    ///
    ///----------------------------------------------------------------------------

#define FLAG(type, name, description, defaultValue, ...) \
        \
        name ## ( ## defaultValue ##), \

    ConfigFlagsTable::ConfigFlagsTable():
        #include "Interface/ConfigFlagsList.h"
#undef FLAG
        nDummy(0)
    {
        for(int i=0; i < FlagCount; flagPresent[i++] = false);

        // set mark for parent flags
        memset((this->flagIsParent),0,(sizeof(this->flagIsParent)));
#define FLAG(type, name, description, defaultValue, parentName, ...) \
        if ((int)parentName##Flag < FlagCount) this->flagIsParent[(int) parentName##Flag] = true;
#include "Interface/ConfigFlagsList.h"
#undef FLAG

        // set all parent flags to their default (setting all child flags to their right values)
        this->SetAllParentFlagsAsDefaultValue();

#if CONFIG_PARSE_CONFIG_FILE
        rawInputFromConfigFileIndex = 0;
        memset(rawInputFromConfigFile, 0, sizeof(rawInputFromConfigFile));
#endif
    }


    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::SetAllParentFlagsAsDefaultValue
    ///
    /// Iterate through all parent flags and set their default value
    ///
    /// Note: only Boolean type supported for now
    ///----------------------------------------------------------------------------

    String *
    ConfigFlagsTable::GetAsString(Flag flag) const
    {
        return reinterpret_cast<String* >(GetProperty(flag));
    }

    Phases *
    ConfigFlagsTable::GetAsPhase(Flag flag) const
    {
        return reinterpret_cast<Phases*>(GetProperty(flag));
    }

    Flag
    ConfigFlagsTable::GetOppositePhaseFlag(Flag flag) const
    {
        switch (flag)
        {
        case OnFlag: return OffFlag;
        case OffFlag: return OnFlag;
        }
        return InvalidFlag;
    }

    Boolean *
    ConfigFlagsTable::GetAsBoolean(Flag flag)  const
    {
        return reinterpret_cast<Boolean*>(GetProperty(flag));
    }

    Number *
    ConfigFlagsTable::GetAsNumber(Flag flag)  const
    {
        return reinterpret_cast<Number* >(GetProperty(flag));
    }

    NumberSet *
    ConfigFlagsTable::GetAsNumberSet(Flag flag)  const
    {
        return reinterpret_cast<NumberSet* >(GetProperty(flag));
    }

    NumberPairSet *
    ConfigFlagsTable::GetAsNumberPairSet(Flag flag)  const
    {
        return reinterpret_cast<NumberPairSet* >(GetProperty(flag));
    }

    NumberTrioSet *
    ConfigFlagsTable::GetAsNumberTrioSet(Flag flag) const
    {
        return reinterpret_cast<NumberTrioSet*>(GetProperty(flag));
    }

    NumberRange *
    ConfigFlagsTable::GetAsNumberRange(Flag flag)  const
    {
        return reinterpret_cast<NumberRange* >(GetProperty(flag));
    }

    void
    ConfigFlagsTable::Enable(Flag flag)
    {
        this->flagPresent[flag] = true;
    }

    void
    ConfigFlagsTable::Disable(Flag flag)
    {
        this->flagPresent[flag] = false;
    }

    bool
    ConfigFlagsTable::IsEnabled(Flag flag)
    {
        return this->flagPresent[flag];
    }

    bool
    ConfigFlagsTable::IsParentFlag(Flag flag) const
    {
        return this->flagIsParent[flag];
    }

    void
    ConfigFlagsTable::SetAllParentFlagsAsDefaultValue()
    {
        for (int i = 0; i < FlagCount; i++)
        {
            Flag currentFlag = (Flag) i;
            if (this->IsParentFlag(currentFlag))
            {
                // only supporting Boolean for now
                AssertMsg(this->GetFlagType(currentFlag) == FlagBoolean, "only supporting boolean flags as parent flags");

                Boolean defaultParentValue = this->GetDefaultValueAsBoolean(currentFlag);
                this->SetAsBoolean(currentFlag, defaultParentValue);
            }
        }
    }

    void ConfigFlagsTable::FinalizeConfiguration()
    {
        TransferAcronymFlagConfiguration();
        TranslateFlagConfiguration();
    }

    void ConfigFlagsTable::TransferAcronymFlagConfiguration()
    {
        // Transfer acronym flag configuration into the corresponding actual flag
    #define FLAG(...)
    #define FLAGNRA(Type, Name, Acronym, ...) \
        if(!IsEnabled(Name##Flag) && IsEnabled(Acronym##Flag)) \
        { \
            Enable(Name##Flag); \
            Name = Acronym; \
        }
    #define FLAGPRA(Type, ParentName, Name, Acronym, ...) \
        if(!IsEnabled(Name##Flag) && IsEnabled(Acronym##Flag)) \
        { \
            Enable(Name##Flag); \
            Name = Acronym; \
        }
        #define FLAGRA(Type, Name, Acronym, ...) FLAGNRA(Type, Name, Acronym, __VA_ARGS__)
    #include "Interface/ConfigFlagsList.h"
    }

    void ConfigFlagsTable::TranslateFlagConfiguration()
    {
        const auto VerifyExecutionModeLimits = [this]()
        {
            const Number zero = static_cast<Number>(0);
            const Number maxUint8 = static_cast<Number>(static_cast<uint8_t>(-1)); // entry point call count is uint8_t
            const Number maxUint16 = static_cast<Number>(static_cast<uint16>(-1));

            Assert(MinInterpretCount >= zero);
            Assert(MinInterpretCount <= maxUint16);
            Assert(MaxInterpretCount >= zero);
            Assert(MaxInterpretCount <= maxUint16);
            Assert(MinSimpleJitRunCount >= zero);
            Assert(MinSimpleJitRunCount <= maxUint8);
            Assert(MaxSimpleJitRunCount >= zero);
            Assert(MaxSimpleJitRunCount <= maxUint8);

            Assert(SimpleJitAfter >= zero);
            Assert(SimpleJitAfter <= maxUint8);
            Assert(FullJitAfter >= zero);
            Assert(FullJitAfter <= maxUint16);

            Assert(AutoProfilingInterpreter0Limit >= zero);
            Assert(AutoProfilingInterpreter0Limit <= maxUint16);
            Assert(ProfilingInterpreter0Limit >= zero);
            Assert(ProfilingInterpreter0Limit <= maxUint16);
            Assert(AutoProfilingInterpreter1Limit >= zero);
            Assert(AutoProfilingInterpreter1Limit <= maxUint16);
            Assert(SimpleJitLimit >= zero);
            Assert(SimpleJitLimit <= maxUint8);
            Assert(ProfilingInterpreter1Limit >= zero);
            Assert(ProfilingInterpreter1Limit <= maxUint16);
            Assert(
                (
                    AutoProfilingInterpreter0Limit +
                    ProfilingInterpreter0Limit +
                    AutoProfilingInterpreter1Limit +
                    SimpleJitLimit +
                    ProfilingInterpreter1Limit
                ) <= maxUint16);
        };
        VerifyExecutionModeLimits();

    #if !DISABLE_JIT
        if(ForceDynamicProfile)
        {
            Force.Enable(DynamicProfilePhase);
        }
        if(ForceJITLoopBody)
        {
            Force.Enable(JITLoopBodyPhase);
        }
    #endif
        if(NoDeferParse)
        {
            Off.Enable(DeferParsePhase);
        }

    #if !DISABLE_JIT
        bool dontEnforceLimitsForSimpleJitAfterOrFullJitAfter = false;
        if((IsEnabled(MinInterpretCountFlag) || IsEnabled(MaxInterpretCountFlag)) &&
            !(IsEnabled(SimpleJitAfterFlag) || IsEnabled(FullJitAfterFlag)))
        {
            if(Off.IsEnabled(SimpleJitPhase))
            {
                Enable(FullJitAfterFlag);
                if(IsEnabled(MaxInterpretCountFlag))
                {
                    FullJitAfter = MaxInterpretCount;
                }
                else
                {
                    FullJitAfter = MinInterpretCount;
                    dontEnforceLimitsForSimpleJitAfterOrFullJitAfter = true;
                }
            }
            else
            {
                Enable(SimpleJitAfterFlag);
                if(IsEnabled(MaxInterpretCountFlag))
                {
                    SimpleJitAfter = MaxInterpretCount;
                }
                else
                {
                    SimpleJitAfter = MinInterpretCount;
                    dontEnforceLimitsForSimpleJitAfterOrFullJitAfter = true;
                }
                if((IsEnabled(MinInterpretCountFlag) && IsEnabled(MinSimpleJitRunCountFlag)) ||
                    IsEnabled(MaxSimpleJitRunCountFlag))
                {
                    Enable(FullJitAfterFlag);
                    FullJitAfter = SimpleJitAfter;
                    if(IsEnabled(MaxSimpleJitRunCountFlag))
                    {
                        FullJitAfter += MaxSimpleJitRunCount;
                    }
                    else
                    {
                        FullJitAfter += MinSimpleJitRunCount;
                        Assert(dontEnforceLimitsForSimpleJitAfterOrFullJitAfter);
                    }
                }
            }
        }

        // Configure execution mode limits
        do
        {
            if(IsEnabled(AutoProfilingInterpreter0LimitFlag) ||
                IsEnabled(ProfilingInterpreter0LimitFlag) ||
                IsEnabled(AutoProfilingInterpreter1LimitFlag) ||
                IsEnabled(SimpleJitLimitFlag) ||
                IsEnabled(ProfilingInterpreter1LimitFlag))
            {
                break;
            }

            if(IsEnabled(ExecutionModeLimitsFlag))
            {
                uint autoProfilingInterpreter0Limit;
                uint profilingInterpreter0Limit;
                uint autoProfilingInterpreter1Limit;
                uint simpleJitLimit;
                uint profilingInterpreter1Limit;
                const int scannedCount =
                    PAL_swscanf(
                        static_cast<const char16_t*>(ExecutionModeLimits),
                        u"%u.%u.%u.%u.%u",
                        &autoProfilingInterpreter0Limit,
                        &profilingInterpreter0Limit,
                        &autoProfilingInterpreter1Limit,
                        &simpleJitLimit,
                        &profilingInterpreter1Limit);
                Assert(scannedCount == 5);

                Enable(AutoProfilingInterpreter0LimitFlag);
                Enable(ProfilingInterpreter0LimitFlag);
                Enable(AutoProfilingInterpreter1LimitFlag);
                Enable(SimpleJitLimitFlag);
                Enable(ProfilingInterpreter1LimitFlag);

                AutoProfilingInterpreter0Limit = autoProfilingInterpreter0Limit;
                ProfilingInterpreter0Limit = profilingInterpreter0Limit;
                AutoProfilingInterpreter1Limit = autoProfilingInterpreter1Limit;
                SimpleJitLimit = simpleJitLimit;
                ProfilingInterpreter1Limit = profilingInterpreter1Limit;
                break;
            }

            if(!NewSimpleJit)
            {
                // Use the defaults for old simple JIT. The flags are not enabled here because the values can be changed later
                // based on other flags, only the defaults values are adjusted here.
                AutoProfilingInterpreter0Limit = DEFAULT_CONFIG_AutoProfilingInterpreter0Limit;
                ProfilingInterpreter0Limit = DEFAULT_CONFIG_ProfilingInterpreter0Limit;
                CompileAssert(
                    DEFAULT_CONFIG_AutoProfilingInterpreter0Limit <= DEFAULT_CONFIG_AutoProfilingInterpreterLimit_OldSimpleJit);
                AutoProfilingInterpreter1Limit =
                    DEFAULT_CONFIG_AutoProfilingInterpreterLimit_OldSimpleJit - DEFAULT_CONFIG_AutoProfilingInterpreter0Limit;
                CompileAssert(DEFAULT_CONFIG_ProfilingInterpreter0Limit <= DEFAULT_CONFIG_SimpleJitLimit_OldSimpleJit);
                SimpleJitLimit = DEFAULT_CONFIG_SimpleJitLimit_OldSimpleJit - DEFAULT_CONFIG_ProfilingInterpreter0Limit;
                ProfilingInterpreter1Limit = 0;
                VerifyExecutionModeLimits();
            }

            if (IsEnabled(SimpleJitAfterFlag))
            {
                Enable(AutoProfilingInterpreter0LimitFlag);
                Enable(ProfilingInterpreter0LimitFlag);
                Enable(AutoProfilingInterpreter1LimitFlag);
                Enable(EnforceExecutionModeLimitsFlag);

                {
                    Js::Number iterationsNeeded = SimpleJitAfter;
                    ProfilingInterpreter0Limit = min(ProfilingInterpreter0Limit, iterationsNeeded);
                    iterationsNeeded -= ProfilingInterpreter0Limit;
                    AutoProfilingInterpreter0Limit = iterationsNeeded;
                    AutoProfilingInterpreter1Limit = 0;
                }

                if(IsEnabled(FullJitAfterFlag))
                {
                    Enable(SimpleJitLimitFlag);
                    Enable(ProfilingInterpreter1LimitFlag);

                    Assert(SimpleJitAfter <= FullJitAfter);
                    Js::Number iterationsNeeded = FullJitAfter - SimpleJitAfter;
                    Js::Number profilingIterationsNeeded =
                        min(NewSimpleJit
                                ? DEFAULT_CONFIG_MinProfileIterations
                                : DEFAULT_CONFIG_MinProfileIterations_OldSimpleJit,
                            FullJitAfter) -
                        ProfilingInterpreter0Limit;
                    if(NewSimpleJit)
                    {
                        ProfilingInterpreter1Limit = min(ProfilingInterpreter1Limit, iterationsNeeded);
                        iterationsNeeded -= ProfilingInterpreter1Limit;
                        profilingIterationsNeeded -= ProfilingInterpreter1Limit;
                        SimpleJitLimit = iterationsNeeded;
                    }
                    else
                    {
                        SimpleJitLimit = iterationsNeeded;
                        profilingIterationsNeeded -= min(SimpleJitLimit, profilingIterationsNeeded);
                        ProfilingInterpreter1Limit = 0;
                    }

                    if(profilingIterationsNeeded != 0)
                    {
                        Js::Number iterationsToMove = min(AutoProfilingInterpreter1Limit, profilingIterationsNeeded);
                        AutoProfilingInterpreter1Limit -= iterationsToMove;
                        ProfilingInterpreter0Limit += iterationsToMove;
                        profilingIterationsNeeded -= iterationsToMove;

                        iterationsToMove = min(AutoProfilingInterpreter0Limit, profilingIterationsNeeded);
                        AutoProfilingInterpreter0Limit -= iterationsToMove;
                        ProfilingInterpreter0Limit += iterationsToMove;
                        profilingIterationsNeeded -= iterationsToMove;

                        Assert(profilingIterationsNeeded == 0);
                    }

                    Assert(
                        (
                            AutoProfilingInterpreter0Limit +
                            ProfilingInterpreter0Limit +
                            AutoProfilingInterpreter1Limit +
                            SimpleJitLimit +
                            ProfilingInterpreter1Limit
                        ) == FullJitAfter);
                }

                Assert(
                    (
                        AutoProfilingInterpreter0Limit +
                        ProfilingInterpreter0Limit +
                        AutoProfilingInterpreter1Limit
                    ) == SimpleJitAfter);
                EnforceExecutionModeLimits = true;
                break;
            }

            if(IsEnabled(FullJitAfterFlag))
            {
                Enable(AutoProfilingInterpreter0LimitFlag);
                Enable(ProfilingInterpreter0LimitFlag);
                Enable(AutoProfilingInterpreter1LimitFlag);
                Enable(SimpleJitLimitFlag);
                Enable(ProfilingInterpreter1LimitFlag);
                Enable(EnforceExecutionModeLimitsFlag);

                Js::Number iterationsNeeded = FullJitAfter;
                if(NewSimpleJit)
                {
                    ProfilingInterpreter1Limit = min(ProfilingInterpreter1Limit, iterationsNeeded);
                    iterationsNeeded -= ProfilingInterpreter1Limit;
                }
                else
                {
                    ProfilingInterpreter1Limit = 0;
                    SimpleJitLimit = min(SimpleJitLimit, iterationsNeeded);
                    iterationsNeeded -= SimpleJitLimit;
                }
                ProfilingInterpreter0Limit = min(ProfilingInterpreter0Limit, iterationsNeeded);
                iterationsNeeded -= ProfilingInterpreter0Limit;
                if(NewSimpleJit)
                {
                    SimpleJitLimit = min(SimpleJitLimit, iterationsNeeded);
                    iterationsNeeded -= SimpleJitLimit;
                }
                AutoProfilingInterpreter0Limit = min(AutoProfilingInterpreter0Limit, iterationsNeeded);
                iterationsNeeded -= AutoProfilingInterpreter0Limit;
                AutoProfilingInterpreter1Limit = iterationsNeeded;

                Assert(
                    (
                        AutoProfilingInterpreter0Limit +
                        ProfilingInterpreter0Limit +
                        AutoProfilingInterpreter1Limit +
                        SimpleJitLimit +
                        ProfilingInterpreter1Limit
                    ) == FullJitAfter);
                EnforceExecutionModeLimits = true;
                break;
            }
            if (IsEnabled(MaxTemplatizedJitRunCountFlag))
            {
                if (MaxTemplatizedJitRunCount >= 0)
                {
                    MinTemplatizedJitRunCount = MaxTemplatizedJitRunCount;
                }
            }
            if (IsEnabled(MaxAsmJsInterpreterRunCountFlag))
            {
                if (MaxAsmJsInterpreterRunCount >= 0)
                {
                    MinAsmJsInterpreterRunCount = MaxAsmJsInterpreterRunCount;
                }
            }

        } while(false);
    #endif

        if( (
            #ifdef ENABLE_PREJIT
                Prejit ||
            #endif
                ForceNative
            ) &&
            !NoNative)
        {
            Enable(AutoProfilingInterpreter0LimitFlag);
            Enable(ProfilingInterpreter0LimitFlag);
            Enable(AutoProfilingInterpreter1LimitFlag);
            Enable(EnforceExecutionModeLimitsFlag);

            // Override any relevant automatic configuration above
            AutoProfilingInterpreter0Limit = 0;
            ProfilingInterpreter0Limit = 0;
            AutoProfilingInterpreter1Limit = 0;
            if(Off.IsEnabled(SimpleJitPhase))
            {
                Enable(SimpleJitLimitFlag);
                Enable(ProfilingInterpreter1LimitFlag);

                SimpleJitLimit = 0;
                ProfilingInterpreter1Limit = 0;
            }

            EnforceExecutionModeLimits = true;
        }

        VerifyExecutionModeLimits();
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetFlag
    ///
    /// Given a string finds the corresponding enum Flag. The comparison is case
    /// in-sensitive
    ///
    ///----------------------------------------------------------------------------

    Flag
    ConfigFlagsTable::GetFlag(const char16_t* str)
    {
        const auto flagStr = chakra_rs::str_helper::to_lowercase(str);
        for(int i=0; i < FlagCount; i++)
        {
            if (flagStr == chakra_rs::str_helper::to_lowercase(FlagNames[i]))
            {
                return Flag(i);
            }
        }
        return InvalidFlag;
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetPhase
    ///
    /// Given a string finds the corresponding enum Phase. The comparison is case
    /// in-sensitive
    ///
    ///----------------------------------------------------------------------------

    Phase
    ConfigFlagsTable::GetPhase(const char16_t* str)
    {
        const auto phaseStr = chakra_rs::str_helper::to_lowercase(str);
        for(int i=0; i < PhaseCount; i++)
        {
            if (phaseStr == chakra_rs::str_helper::to_lowercase(PhaseNames[i]))
            {
                return Phase(i);
            }
        }
        return InvalidPhase;
    }

    void
    ConfigFlagsTable::PrintUsageString()
    {
        Output::Print(u"List of Phases:\n");
        for(int i = 0; i < PhaseCount; i++)
        {
            if (i % 4 == 0)
            {
                Output::Print(u"\n  ");
            }
            Output::Print(u"%-40ls ", PhaseNames[i]);
        }

        Output::Print(u"\n\nList of flags:\n\n");
        for(int i = 0; i < FlagCount; i++)
        {
            Output::Print(u"%60ls ", FlagNames[i]);
            switch(GetFlagType(Flag(i)))
            {
            case InvalidFlagType:
                break;
            case FlagString:
                Output::Print(u"[:String]        ");
                break;
            case FlagPhases:
                Output::Print(u"[:Phase]         ");
                break;
            case FlagNumber:
                Output::Print(u"[:Number]        ");
                break;
            case FlagBoolean:
                Output::Print(u"                 ");
                break;
            case FlagNumberSet:
                Output::Print(u"[:NumberSet]     ");
                break;
            case FlagNumberPairSet:
                Output::Print(u"[:NumberPairSet] ");
                break;
            case FlagNumberTrioSet:
                Output::Print(u"[:NumberTrioSet] ");
                break;
            case FlagNumberRange:
                Output::Print(u"[:NumberRange]   ");
                break;
            default:
                Assert(false);
            }
            Output::Print(u"%ls\n", FlagDescriptions[i]);
        }
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetFlagType
    ///
    /// Given a flag it returns the type (PhaseFlag, StringFlag ...). This could
    /// easily have been a lookup table like FlagNames and PhaseNames but this
    /// seems more concise
    ///
    ///----------------------------------------------------------------------------


    FlagTypes
    ConfigFlagsTable::GetFlagType(Flag flag)
    {
        switch(flag)
        {
    #define FLAG(type, name, ...) \
            case name##Flag : \
                return Flag##type; \

    #include "Interface/ConfigFlagsList.h"

            default:
                return InvalidFlagType;
        }
    }


    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetProperty
    ///
    /// Get the field corresponding to the flag. used as an internal method for
    /// the various GetAs* methods.
    ///
    ///----------------------------------------------------------------------------


    void *
    ConfigFlagsTable::GetProperty(Flag flag) const
    {
        switch(flag)
        {
        #if DBG
        case ArrayValidateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ArrayValidate));
        case MemOpMissingValueValidateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemOpMissingValueValidate));
        case OOPJITFixupValidateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&OOPJITFixupValidate));
        #endif
        #ifdef ARENA_MEMORY_VERIFY
        case ArenaNoFreeListFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ArenaNoFreeList));
        case ArenaNoPageReuseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ArenaNoPageReuse));
        case ArenaUseHeapAllocFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ArenaUseHeapAlloc));
        #endif
        case ValidateInlineStackFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ValidateInlineStack));
        case AsmDiffFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsmDiff));
        case AsmDumpModeFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&AsmDumpMode));
        case AsmJsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsmJs));
        case AsmJsStopOnErrorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsmJsStopOnError));
        case AsmJsEdgeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsmJsEdge));
        case WasmFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Wasm));
        case WasmI64Flag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmI64));
        case WasmFastArrayFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmFastArray));
        case WasmSharedArrayVirtualBufferFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmSharedArrayVirtualBuffer));
        case WasmMathExFilterFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmMathExFilter));
        case WasmCheckVersionFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmCheckVersion));
        case WasmAssignModuleIDFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmAssignModuleID));
        case WasmIgnoreLimitsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmIgnoreLimits));
        case WasmFoldFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmFold));
        case WasmIgnoreResponseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmIgnoreResponse));
        case WasmMaxTableSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&WasmMaxTableSize));
        case WasmThreadsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmThreads));
        case WasmMultiValueFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmMultiValue));
        case WasmSignExtendsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmSignExtends));
        case WasmNontrappingFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmNontrapping));

        // WebAssembly Experimental Features
        // Master WasmExperimental flag to activate WebAssembly experimental features
        case WasmExperimentalFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmExperimental));

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        case WasmSimdFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmSimd));

        case AssertBreakFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AssertBreak));
        case AssertPopUpFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AssertPopUp));
        case AssertIgnoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AssertIgnore));
        case AsyncDebuggingFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsyncDebugging));
        case BailOnNoProfileLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BailOnNoProfileLimit));
        case BailOnNoProfileRejitLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BailOnNoProfileRejitLimit));
        case BaselineModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BaselineMode));
        case DumpOnCrashFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DumpOnCrash));
        case FullMemoryDumpFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&FullMemoryDump));
        #ifdef BAILOUT_INJECTION
        case BailOutFlag:
            return reinterpret_cast<void*>(const_cast<NumberPairSet*>(&BailOut));
        case BailOutAtEveryLineFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BailOutAtEveryLine));
        case BailOutAtEveryByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BailOutAtEveryByteCode));
        case BailOutAtEveryImplicitCallFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BailOutAtEveryImplicitCall));
        case BailOutByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<NumberSet*>(&BailOutByteCode));
        #endif
        case BenchmarkFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Benchmark));
        case BgJitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BgJit));
        case BgParseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BgParse));
        case BgJitDelayFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BgJitDelay));
        case BgJitDelayFgBufferFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BgJitDelayFgBuffer));
        case BgJitPendingFuncCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BgJitPendingFuncCap));

        case CreateFunctionProxyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CreateFunctionProxy));
        case HybridFgJitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&HybridFgJit));
        case HybridFgJitBgQueueLengthThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&HybridFgJitBgQueueLengthThreshold));
        case BytecodeHistFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BytecodeHist));
        case CurrentSourceInfoFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CurrentSourceInfo));
        case CFGLogFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CFGLog));
        case CheckAlignmentFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CheckAlignment));
        case CheckEmitBufferPermissionsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CheckEmitBufferPermissions));
        #ifdef CHECK_MEMORY_LEAK
        case CheckMemoryLeakFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CheckMemoryLeak));
        case DumpOnLeakFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DumpOnLeak));
        #endif
        case CheckOpHelpersFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CheckOpHelpers));
        case CloneInlinedPolymorphicCachesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CloneInlinedPolymorphicCaches));
        case ConcurrentRuntimeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ConcurrentRuntime));
        case ConstructorInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ConstructorInlineThreshold));
        case ConstructorCallsRequiredToFinalizeCachedTypeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ConstructorCallsRequiredToFinalizeCachedType));
        case PropertyCacheMissPenaltyFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PropertyCacheMissPenalty));
        case PropertyCacheMissThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PropertyCacheMissThreshold));
        case PropertyCacheMissResetFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PropertyCacheMissReset));
        case DebugFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Debug));
        case DebugBreakFlag:
            return reinterpret_cast<void*>(const_cast<NumberSet*>(&DebugBreak));
        case StatementDebugBreakFlag:
            return reinterpret_cast<void*>(const_cast<NumberTrioSet*>(&StatementDebugBreak));
        case DebugBreakOnPhaseBeginFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&DebugBreakOnPhaseBegin));

        case DebugWindowFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DebugWindow));
        case ParserStateCacheFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ParserStateCache));
        case CompressParserStateCacheFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CompressParserStateCache));
        case DeferTopLevelTillFirstCallFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DeferTopLevelTillFirstCall));
        case DeferParseFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&DeferParse));
        case DirectCallTelemetryStatsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DirectCallTelemetryStats));
        case DisableArrayBTreeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisableArrayBTree));
        case DisableRentalThreadingFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisableRentalThreading));
        case DisableVTuneSourceLineInfoFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisableVTuneSourceLineInfo));
        case DisplayMemStatsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisplayMemStats));
        case DumpFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Dump));
        #ifdef DUMP_FRAGMENTATION_STATS
        case DumpFragmentationStatsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpFragmentationStats));
        #endif
        case DumpIRAddressesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpIRAddresses));
        case DumpLineNoInColorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpLineNoInColor));
        #ifdef RECYCLER_DUMP_OBJECT_GRAPH
        case DumpObjectGraphOnExitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpObjectGraphOnExit));
        case DumpObjectGraphOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpObjectGraphOnCollect));
        #endif
        case DumpEvalStringOnRemovalFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpEvalStringOnRemoval));
        case DumpObjectGraphOnEnumFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpObjectGraphOnEnum));
        #ifdef DYNAMIC_PROFILE_STORAGE
        case DynamicProfileCacheFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DynamicProfileCache));
        case DpcFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Dpc));
        case DynamicProfileCacheDirFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DynamicProfileCacheDir));
        case DynamicProfileInputFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DynamicProfileInput));
        case DpiFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Dpi));
        #endif
        #ifdef EDIT_AND_CONTINUE
        case EditTestFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EditTest));
        #endif
        case WininetProfileCacheFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WininetProfileCache));
        case NoDynamicProfileInMemoryCacheFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoDynamicProfileInMemoryCache));
        case ProfileBasedSpeculativeJitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileBasedSpeculativeJit));
        case ProfileBasedSpeculationCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfileBasedSpeculationCap));
        case ExecuteByteCodeBufferReturnsInvalidByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ExecuteByteCodeBufferReturnsInvalidByteCode));
        case ExpirableCollectionGCCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ExpirableCollectionGCCount));
        case ExpirableCollectionTriggerThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ExpirableCollectionTriggerThreshold));
        case SkipSplitOnNoResultFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&SkipSplitOnNoResult));
        case Force32BitByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Force32BitByteCode));

        case CollectGarbageFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CollectGarbage));

        case IntlFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Intl));
        case IntlBuiltInsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&IntlBuiltIns));
        case IntlPlatformFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&IntlPlatform));

        case JsBuiltInFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&JsBuiltIn));
        case JitReproFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&JitRepro));
        case EntryPointInfoRpcDataFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EntryPointInfoRpcData));

        case LdChakraLibFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LdChakraLib));
        case TestChakraLibFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TestChakraLib));

        // ES6 (BLUE+1) features/flags

        // Master ES6 flag to enable STABLE ES6 features/flags
        case ES6Flag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6));

        // Master ES6 flag to enable ALL sub ES6 features/flags
        case ES6AllFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6All));

        // Master ES6 flag to enable Threshold ES6 features/flags
        case ES6ExperimentalFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Experimental));

        // Per ES6 feature/flag

        case ES7AsyncAwaitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES7AsyncAwait));
        case ES6DateParseFixFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6DateParseFix));
        case ES6FunctionNameFullFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6FunctionNameFull));
        case ES6GeneratorsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Generators));
        case ES7ExponentiationOperatorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES7ExponentiationOperator));

        case ES7ValuesEntriesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES7ValuesEntries));
        case ES7TrailingCommaFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES7TrailingComma));
        case ES6IsConcatSpreadableFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6IsConcatSpreadable));
        case ES6MathFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Math));

        #ifndef COMPILE_DISABLE_ESDynamicImport
        #define COMPILE_DISABLE_ESDynamicImport 0
        #endif
        case ESDynamicImportFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESDynamicImport));

        case ES6ModuleFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Module));
        case ES6ObjectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Object));
        case ES6NumberFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Number));
        case ES6ObjectLiteralsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6ObjectLiterals));
        case ES6ProxyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Proxy));
        case ES6RestFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Rest));
        case ES6SpreadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Spread));
        case ES6StringFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6String));
        case ES6StringPrototypeFixesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6StringPrototypeFixes));
        case ES2018ObjectRestSpreadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES2018ObjectRestSpread));

        case ES6PrototypeChainFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6PrototypeChain));
        case ES6ToPrimitiveFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6ToPrimitive));
        case ES6ToLengthFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6ToLength));
        case ES6ToStringTagFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6ToStringTag));
        case ES6UnicodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Unicode));
        case ES6UnicodeVerboseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6UnicodeVerbose));
        case ES6UnscopablesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Unscopables));
        case ES6RegExStickyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6RegExSticky));
        case ES2018RegExDotAllFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES2018RegExDotAll));
        case ESExportNsAsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESExportNsAs));
        case ES2018AsyncIterationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES2018AsyncIteration));
        case ESTopLevelAwaitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESTopLevelAwait));

        #ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
            #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
        #endif
        case ES6RegExPrototypePropertiesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6RegExPrototypeProperties));

        #ifndef COMPILE_DISABLE_ES6RegExSymbols
            #define COMPILE_DISABLE_ES6RegExSymbols 0
        #endif

        // When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
        // sets implicit call flag before calling into script
        // Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        case ES6RegExSymbolsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6RegExSymbols));

        case ES6VerboseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Verbose));
        case ESObjectGetOwnPropertyDescriptorsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESObjectGetOwnPropertyDescriptors));

        #ifndef COMPILE_DISABLE_ESSharedArrayBuffer
            #define COMPILE_DISABLE_ESSharedArrayBuffer 0
        #endif

        case ESSharedArrayBufferFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESSharedArrayBuffer));

        // Newer language feature flags

        // ES BigInt flag
        case ESBigIntFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESBigInt));

        // ES Numeric Separator support for numeric constants
        case ESNumericSeparatorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESNumericSeparator));

        // ES Nullish coalescing operator support (??)
        case ESNullishCoalescingOperatorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESNullishCoalescingOperator));

        // ES Hashbang support for interpreter directive syntax
        case ESHashbangFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESHashbang));

        // ES Symbol.prototype.description flag
        case ESSymbolDescriptionFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESSymbolDescription));

        case ESArrayFindFromLastFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESArrayFindFromLast));

        // ES Promise.any and AggregateError flag
        case ESPromiseAnyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESPromiseAny));

        // ES import.meta keyword meta-property
        case ESImportMetaFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESImportMeta));

        //ES globalThis flag
        case ESGlobalThisFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESGlobalThis));

        // This flag to be removed once JITing generator functions is stable
        case JitES6GeneratorsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&JitES6Generators));

        case FastLineColumnCalculationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&FastLineColumnCalculation));
        case FilenameFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Filename));
        case FreeRejittedCodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&FreeRejittedCode));
        case ForceGuardPagesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceGuardPages));
        case PrintGuardPageBoundsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintGuardPageBounds));
        case ForceLegacyEngineFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceLegacyEngine));
        case ForceFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Force));
        case StressFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Stress));
        case ForceArrayBTreeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceArrayBTree));
        case StrongArraySortFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&StrongArraySort));
        case ForceCleanPropertyOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceCleanPropertyOnCollect));
        case ForceCleanCacheOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceCleanCacheOnCollect));
        case ForceGCAfterJSONParseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceGCAfterJSONParse));
        case ForceDecommitOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceDecommitOnCollect));
        case ForceDeferParseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceDeferParse));
        case ForceDiagnosticsModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceDiagnosticsMode));
        case ForceGetWriteWatchOOMFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceGetWriteWatchOOM));
        case ForcePostLowerGlobOptInstrStringFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForcePostLowerGlobOptInstrString));
        case ForceSplitScopeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceSplitScope));
        case EnumerateSpecialPropertiesInDebuggerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnumerateSpecialPropertiesInDebugger));
        case EnableContinueAfterExceptionWrappersForHelpersFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableContinueAfterExceptionWrappersForHelpers));
        case EnableContinueAfterExceptionWrappersForBuiltInsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableContinueAfterExceptionWrappersForBuiltIns));
        case EnableFunctionSourceReportForHeapEnumFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableFunctionSourceReportForHeapEnum));
        case ForceFragmentAddressSpaceFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ForceFragmentAddressSpace));
        case ForceOOMOnEBCommitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ForceOOMOnEBCommit));
        case ForceDynamicProfileFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceDynamicProfile));
        case ForceES5ArrayFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceES5Array));
        case ForceAsmJsLinkFailFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceAsmJsLinkFail));
        case ForceExpireOnNonCacheCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceExpireOnNonCacheCollect));
        case ForceFastPathFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceFastPath));
        case ForceFloatPrefFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceFloatPref));
        case ForceJITLoopBodyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceJITLoopBody));
        case ForceStaticInterpreterThunkFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceStaticInterpreterThunk));
        case DumpCommentsFromReferencedFilesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpCommentsFromReferencedFiles));
        case DelayFullJITSmallFuncFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&DelayFullJITSmallFunc));
        case EnableFatalErrorOnOOMFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableFatalErrorOnOOM));

        #if defined(_M_ARM32_OR_ARM64)
        case ForceLocalsPtrFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceLocalsPtr));
        #endif
        case DeferLoadingAvailableSourceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DeferLoadingAvailableSource));
        case ForceNativeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceNative));
        case ForceSerializedFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceSerialized));
        case ForceSerializedBytecodeMajorVersionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ForceSerializedBytecodeMajorVersion));
        case ForceSerializedBytecodeVersionSchemaFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ForceSerializedBytecodeVersionSchema));
        case ForceStrictModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceStrictMode));
        case ForceUndoDeferFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceUndoDefer));
        case ForceBlockingConcurrentCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceBlockingConcurrentCollect));
        case FreTestDiagModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&FreTestDiagMode));
        #ifdef BYTECODE_TESTING
        case ByteCodeBranchLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ByteCodeBranchLimit));
        case MediumByteCodeLayoutFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MediumByteCodeLayout));
        case LargeByteCodeLayoutFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LargeByteCodeLayout));
        #endif
        case InduceCodeGenFailureFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InduceCodeGenFailure));
        case InduceCodeGenFailureSeedFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InduceCodeGenFailureSeed));
        case InjectPartiallyInitializedInterpreterFrameErrorFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InjectPartiallyInitializedInterpreterFrameError));
        case InjectPartiallyInitializedInterpreterFrameErrorTypeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InjectPartiallyInitializedInterpreterFrameErrorType));
        case GenerateByteCodeBufferReturnsCantGenerateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&GenerateByteCodeBufferReturnsCantGenerate));
        case GoptCleanupThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&GoptCleanupThreshold));
        case AsmGoptCleanupThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AsmGoptCleanupThreshold));
        case HighPrecisionDateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&HighPrecisionDate));
        case InlineCountMaxFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineCountMax));
        case InlineCountMaxInLoopBodiesFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineCountMaxInLoopBodies));
        case icminlbFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&icminlb));
        case InlineInLoopBodyScaleDownFactorFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineInLoopBodyScaleDownFactor));
        case iilbsdfFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&iilbsdf));
        case InlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineThreshold));
        case AggressiveInlineCountMaxFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AggressiveInlineCountMax));
        case AggressiveInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AggressiveInlineThreshold));
        case InlineThresholdAdjustCountInLargeFunctionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineThresholdAdjustCountInLargeFunction));
        case InlineThresholdAdjustCountInMediumSizedFunctionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineThresholdAdjustCountInMediumSizedFunction));
        case InlineThresholdAdjustCountInSmallFunctionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineThresholdAdjustCountInSmallFunction));
        case AsmJsInlineAdjustFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AsmJsInlineAdjust));
        case InterpretFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Interpret));
        case InstrumentFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Instrument));
        case JitQueueThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&JitQueueThreshold));
        #ifdef LEAK_REPORT
        case LeakReportFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&LeakReport));
        #endif
        case LoopInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopInlineThreshold));
        case LeafInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LeafInlineThreshold));
        case ConstantArgumentInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ConstantArgumentInlineThreshold));
        case RecursiveInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecursiveInlineThreshold));
        case RecursiveInlineDepthMaxFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecursiveInlineDepthMax));
        case RecursiveInlineDepthMinFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecursiveInlineDepthMin));
        case RedeferralCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RedeferralCap));
        case LoopFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Loop));
        case LoopInterpretCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopInterpretCount));
        case licFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&lic));
        case LoopProfileIterationsFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopProfileIterations));
        case OutsideLoopInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&OutsideLoopInlineThreshold));
        case MaxFuncInlineDepthFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxFuncInlineDepth));
        case MaxNumberOfInlineesWithLoopFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxNumberOfInlineesWithLoop));
        #ifdef MEMSPECT_TRACKING
        case MemspectFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Memspect));
        #endif
        case PolymorphicInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PolymorphicInlineThreshold));
        case PrimeRecyclerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrimeRecycler));
        case TraceEngineRefcountFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceEngineRefcount));
        #if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
        case LeakStackTraceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LeakStackTrace));
        case ForceMemoryLeakFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceMemoryLeak));
        #endif
        case DumpAfterFinalGCFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpAfterFinalGC));
        case ForceOldDateAPIFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceOldDateAPI));

        case JitLoopBodyHotLoopThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&JitLoopBodyHotLoopThreshold));
        case LoopBodySizeThresholdToDisableOptsFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopBodySizeThresholdToDisableOpts));

        case MaxJitThreadCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxJitThreadCount));
        case ForceMaxJitThreadCountFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceMaxJitThreadCount));

        case MitigateSpectreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MitigateSpectre));

        case AddMaskingBlocksFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AddMaskingBlocks));

        case PoisonVarArrayLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonVarArrayLoad));
        case PoisonIntArrayLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonIntArrayLoad));
        case PoisonFloatArrayLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonFloatArrayLoad));
        case PoisonTypedArrayLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonTypedArrayLoad));
        case PoisonStringLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonStringLoad));
        case PoisonObjectsForLoadsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonObjectsForLoads));

        case PoisonVarArrayStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonVarArrayStore));
        case PoisonIntArrayStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonIntArrayStore));
        case PoisonFloatArrayStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonFloatArrayStore));
        case PoisonTypedArrayStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonTypedArrayStore));
        case PoisonStringStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonStringStore));
        case PoisonObjectsForStoresFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonObjectsForStores));

        case MinInterpretCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinInterpretCount));
        case MinSimpleJitRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinSimpleJitRunCount));
        case MaxInterpretCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxInterpretCount));
        case MicFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Mic));
        case MaxSimpleJitRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxSimpleJitRunCount));
        case MsjrcFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Msjrc));
        case MinMemOpCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinMemOpCount));
        case MmocFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Mmoc));

        #if ENABLE_COPYONACCESS_ARRAY
        case MaxCopyOnAccessArrayLengthFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxCopyOnAccessArrayLength));
        case MinCopyOnAccessArrayLengthFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinCopyOnAccessArrayLength));
        case CopyOnAccessArraySegmentCacheSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&CopyOnAccessArraySegmentCacheSize));
        #endif

        case MinTemplatizedJitRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinTemplatizedJitRunCount));
        case MinAsmJsInterpreterRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinAsmJsInterpreterRunCount));

        case MinTemplatizedJitLoopRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinTemplatizedJitLoopRunCount));
        case MaxTemplatizedJitRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxTemplatizedJitRunCount));
        case MtjrcFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Mtjrc));
        case MaxAsmJsInterpreterRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxAsmJsInterpreterRunCount));
        case MaicFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Maic));

        case AutoProfilingInterpreter0LimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AutoProfilingInterpreter0Limit));
        case ProfilingInterpreter0LimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfilingInterpreter0Limit));
        case AutoProfilingInterpreter1LimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AutoProfilingInterpreter1Limit));
        case SimpleJitLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SimpleJitLimit));
        case ProfilingInterpreter1LimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfilingInterpreter1Limit));

        case ExecutionModeLimitsFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&ExecutionModeLimits));
        case EmlFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Eml));
        case EnforceExecutionModeLimitsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnforceExecutionModeLimits));
        case EemlFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Eeml));

        case SimpleJitAfterFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SimpleJitAfter));
        case SjaFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Sja));
        case FullJitAfterFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&FullJitAfter));
        case FjaFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Fja));

        case NewSimpleJitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NewSimpleJit));

        case MaxLinearIntCaseCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxLinearIntCaseCount));
        case MaxSingleCharStrJumpTableSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxSingleCharStrJumpTableSize));
        case MaxSingleCharStrJumpTableRatioFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxSingleCharStrJumpTableRatio));
        case MinSwitchJumpTableSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinSwitchJumpTableSize));
        case MaxLinearStringCaseCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxLinearStringCaseCount));
        case MinDeferredFuncTokenCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinDeferredFuncTokenCount));
        #if DBG
        case SkipFuncCountForBailOnNoProfileFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SkipFuncCountForBailOnNoProfile));
        #endif
        case MaxJITFunctionBytecodeByteLengthFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxJITFunctionBytecodeByteLength));
        case MaxJITFunctionBytecodeCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxJITFunctionBytecodeCount));
        case MaxLoopsPerFunctionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxLoopsPerFunction));
        case FuncObjectInlineCacheThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&FuncObjectInlineCacheThreshold));
        case NoDeferParseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoDeferParse));
        case NoLogoFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoLogo));
        case OOPJITMissingOptsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&OOPJITMissingOpts));
        case CrashOnOOPJITFailureFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CrashOnOOPJITFailure));
        case OOPCFGRegistrationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&OOPCFGRegistration));
        case ForceJITCFGCheckFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceJITCFGCheck));
        case UseJITTrampolineFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&UseJITTrampoline));
        case NoNativeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoNative));
        case NopFrequencyFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&NopFrequency));
        case NoStrictModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoStrictMode));
        case NormalizeStatsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NormalizeStats));
        case OffFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Off));
        case OffProfiledByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&OffProfiledByteCode));
        case OnFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&On));
        case OutputFileFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&OutputFile));
        case OutputFileOpenModeFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&OutputFileOpenMode));
        #ifdef ENABLE_TRACE
        case InMemoryTraceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&InMemoryTrace));
        case InMemoryTraceBufferSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InMemoryTraceBufferSize));
        #ifdef STACK_BACK_TRACE
        case TraceWithStackFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceWithStack));
        #endif // STACK_BACK_TRACE
        #endif // ENABLE_TRACE
        case PrintRunTimeDataCollectionTraceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintRunTimeDataCollectionTrace));
        #ifdef ENABLE_PREJIT
        case PrejitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Prejit));
        #endif
        case PrintSrcInDumpFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintSrcInDump));
        #if PROFILE_DICTIONARY
        case ProfileDictionaryFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfileDictionary));
        #endif
        #ifdef PROFILE_EXEC
        case ProfileFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Profile));
        case ProfileThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfileThreshold));
        #endif
        #ifdef PROFILE_OBJECT_LITERALS
        case ProfileObjectLiteralFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileObjectLiteral));
        #endif
        #ifdef PROFILE_MEM
        case ProfileMemoryFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&ProfileMemory));
        #endif
        #ifdef PROFILE_STRINGS
        case ProfileStringsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileStrings));
        #endif
        #ifdef PROFILE_TYPES
        case ProfileTypesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileTypes));
        #endif
        #ifdef PROFILE_EVALMAP
        case ProfileEvalMapFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileEvalMap));
        #endif

        #ifdef PROFILE_BAILOUT_RECORD_MEMORY
        case ProfileBailOutRecordMemoryFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileBailOutRecordMemory));
        #endif

        #if DBG
        case ValidateIntRangesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ValidateIntRanges));
        #endif
        case RejitMaxBailOutCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RejitMaxBailOutCount));
        case CallsToBailoutsRatioForRejitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&CallsToBailoutsRatioForRejit));
        case LoopIterationsToBailoutsRatioForRejitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopIterationsToBailoutsRatioForRejit));
        case MinBailOutsBeforeRejitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinBailOutsBeforeRejit));
        case MinBailOutsBeforeRejitForLoopsFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinBailOutsBeforeRejitForLoops));
        case LibraryStackFrameFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LibraryStackFrame));
        case LibraryStackFrameDebuggerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LibraryStackFrameDebugger));
        #ifdef RECYCLER_STRESS
        case RecyclerStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerStress));
        #if ENABLE_CONCURRENT_GC
        case RecyclerBackgroundStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerBackgroundStress));
        case RecyclerConcurrentStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerConcurrentStress));
        case RecyclerConcurrentRepeatStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerConcurrentRepeatStress));
        #endif
        #if ENABLE_PARTIAL_GC
        case RecyclerPartialStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerPartialStress));
        #endif
        case RecyclerTrackStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerTrackStress));
        case RecyclerInduceFalsePositivesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerInduceFalsePositives));
        #endif // RECYCLER_STRESS
        case RecyclerForceMarkInteriorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerForceMarkInterior));
        #if ENABLE_CONCURRENT_GC
        case RecyclerPriorityBoostTimeoutFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecyclerPriorityBoostTimeout));
        case RecyclerThreadCollectTimeoutFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecyclerThreadCollectTimeout));
        case EnableConcurrentSweepAllocFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableConcurrentSweepAlloc));
        case ecsaFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ecsa));
        #endif
        #ifdef RECYCLER_PAGE_HEAP
        case PageHeapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PageHeap));
        case PageHeapAllocStackFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PageHeapAllocStack));
        case PageHeapFreeStackFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PageHeapFreeStack));
        case PageHeapBucketNumberFlag:
            return reinterpret_cast<void*>(const_cast<NumberRange*>(&PageHeapBucketNumber));
        case PageHeapBlockTypeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PageHeapBlockType));
        case PageHeapDecommitGuardPageFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PageHeapDecommitGuardPage));
        #endif
        #ifdef RECYCLER_NO_PAGE_REUSE
        case RecyclerNoPageReuseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerNoPageReuse));
        #endif
        #ifdef RECYCLER_MEMORY_VERIFY
        case RecyclerVerifyFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&RecyclerVerify));
        case RecyclerVerifyPadSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecyclerVerifyPadSize));
        #endif
        case RecyclerTestFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerTest));
        case RecyclerProtectPagesOnRescanFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerProtectPagesOnRescan));
        #ifdef RECYCLER_VERIFY_MARK
        case RecyclerVerifyMarkFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerVerifyMark));
        #endif
        case LowMemoryCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LowMemoryCap));
        case NewPagesCapDuringBGSweepingFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&NewPagesCapDuringBGSweeping));
        case AllocPolicyLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AllocPolicyLimit));
        #ifdef RUNTIME_DATA_COLLECTION
        case RuntimeDataOutputFileFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&RuntimeDataOutputFile));
        #endif
        case SpeculationCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SpeculationCap));
        #if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        case StatsFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Stats));
        #endif
        #if EXCEPTION_RECOVERY
        case SwallowExceptionsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&SwallowExceptions));
        #endif
        case PrintSystemExceptionFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintSystemException));
        case SwitchOptHolesThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SwitchOptHolesThreshold));
        case TempMinFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&TempMin));
        case TempMaxFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&TempMax));
        case TraceFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Trace));

        #if defined(_M_X64)
        case LoopAlignNopLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopAlignNopLimit));
        #endif

        #ifdef PROFILE_MEM
        case TraceMemoryFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&TraceMemory));
        #endif
        #if DBG_DUMP || defined(RECYCLER_TRACE)
        //TraceMetaDataParsing flag with optional levels:
        //    Level 1 = interfaces only
        //    Level 2 = interfaces and methods
        //    Level 3 = interfaces, methods and parameters
        //    Level 4 = interfaces and properties
        //    Level 5 (default) = ALL
        case TraceMetaDataParsingFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&TraceMetaDataParsing));
        case TraceWin8AllocationsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceWin8Allocations));
        case TraceWin8DeallocationsImmediateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceWin8DeallocationsImmediate));
        case PrintWin8StatsDetailedFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintWin8StatsDetailed));
        case TraceProtectPagesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceProtectPages));
        #endif
        case TraceAsyncDebugCallsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceAsyncDebugCalls));
        #ifdef TRACK_DISPATCH
        case TrackDispatchFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TrackDispatch));
        #endif
        case VerboseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Verbose));
        case UseFullNameFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&UseFullName));
        case Utf8Flag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Utf8));
        case VersionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Version));
        case WERExceptionSupportFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WERExceptionSupport));
        case ExtendedErrorStackForTestHostFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ExtendedErrorStackForTestHost));
        case errorStackTraceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&errorStackTrace));
        case DoHeapEnumOnEngineShutdownFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DoHeapEnumOnEngineShutdown));
        #ifdef HEAP_ENUMERATION_VALIDATION
        case ValidateHeapEnumFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ValidateHeapEnum));
        #endif

        #if ENABLE_REGEX_CONFIG_OPTIONS
        //
        // Regex flags
        //
        case RegexTracingFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexTracing));
        case RegexProfileFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexProfile));
        case RegexDebugFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexDebug));
        case RegexDebugASTFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexDebugAST));
        case RegexDebugAnnotatedASTFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexDebugAnnotatedAST));
        case RegexBytecodeDebugFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexBytecodeDebug));
        case RegexOptimizeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexOptimize));
        case DynamicRegexMruListSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&DynamicRegexMruListSize));
        #endif

        case OptimizeForManyInstancesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&OptimizeForManyInstances));
        case EnableArrayTypeMutationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableArrayTypeMutation));
        case ArrayMutationTestSeedFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ArrayMutationTestSeed));
        case TestTraceFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&TestTrace));
        case EnableEvalMapCleanupFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableEvalMapCleanup));
        #ifdef PROFILE_MEM
        case TraceObjectAllocationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceObjectAllocation));
        #endif
        case SseFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Sse));
        case DeletedPropertyReuseThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&DeletedPropertyReuseThreshold));
        case ForceStringKeyedSimpleDictionaryTypeHandlerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceStringKeyedSimpleDictionaryTypeHandler));
        case BigDictionaryTypeHandlerThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BigDictionaryTypeHandlerThreshold));
        case TypeSnapshotEnumerationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TypeSnapshotEnumeration));
        case IsolatePrototypesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&IsolatePrototypes));
        case ChangeTypeOnProtoFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ChangeTypeOnProto));
        case ShareInlineCachesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ShareInlineCaches));
        case DisableDebugObjectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisableDebugObject));
        case DumpHeapFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpHeap));
        case autoProxyFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&autoProxy));
        case PerfHintLevelFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PerfHintLevel));
        #ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        case MemProtectHeapFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeap));
        #endif
        #ifdef RECYCLER_STRESS
        case MemProtectHeapStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapStress));
        #if ENABLE_CONCURRENT_GC
        case MemProtectHeapBackgroundStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapBackgroundStress));
        case MemProtectHeapConcurrentStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapConcurrentStress));
        case MemProtectHeapConcurrentRepeatStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapConcurrentRepeatStress));
        #endif
        #if ENABLE_PARTIAL_GC
        case MemProtectHeapPartialStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapPartialStress));
        #endif
        #endif
        #ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        case FixPropsOnPathTypesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&FixPropsOnPathTypes));
        #endif
        case BailoutTraceFilterFlag:
            return reinterpret_cast<void*>(const_cast<NumberSet*>(&BailoutTraceFilter));
        case RejitTraceFilterFlag:
            return reinterpret_cast<void*>(const_cast<NumberSet*>(&RejitTraceFilter));

        // recycler heuristic flags
        case MaxBackgroundFinishMarkCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxBackgroundFinishMarkCount));
        case BackgroundFinishMarkWaitTimeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BackgroundFinishMarkWaitTime));
        case MinBackgroundRepeatMarkRescanBytesFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinBackgroundRepeatMarkRescanBytes));

        #if defined(_M_X64)
        case ZeroMemoryWithNonTemporalStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ZeroMemoryWithNonTemporalStore));
        #endif

        // recycler memory restrict test flags
        case MaxMarkStackPageCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxMarkStackPageCount));
        case MaxTrackedObjectListCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxTrackedObjectListCount));

        // make the recycler page integration path easier to hit
        case NumberAllocPlusSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&NumberAllocPlusSize));

        #if DBG
        case InitializeInterpreterSlotsWithInvalidStackVarFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&InitializeInterpreterSlotsWithInvalidStackVar));
        #endif

        #if DBG
        case PRNGSeed0Flag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PRNGSeed0));
        case PRNGSeed1Flag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PRNGSeed1));
        #endif

        case ClearInlineCachesOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ClearInlineCachesOnCollect));
        case InlineCacheInvalidationListCompactionThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineCacheInvalidationListCompactionThreshold));
        case ConstructorCacheInvalidationThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ConstructorCacheInvalidationThreshold));

        #ifdef IR_VIEWER
        case IRViewerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&IRViewer));
        #endif /* IR_VIEWER */

        case GCMemoryThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&GCMemoryThreshold));

        #if DBG
            case SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SimulatePolyCacheWithOneTypeForInlineCacheIndex));
        #endif

        case JITServerIdleTimeoutFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&JITServerIdleTimeout));
        case JITServerMaxInactivePageAllocatorCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&JITServerMaxInactivePageAllocatorCount));

        case StrictWriteBarrierCheckFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&StrictWriteBarrierCheck));
        case WriteBarrierTestFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WriteBarrierTest));
        case ForceSoftwareWriteBarrierFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceSoftwareWriteBarrier));
        case VerifyBarrierBitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&VerifyBarrierBit));
        case EnableBGFreeZeroFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableBGFreeZero));
        case KeepRecyclerTrackDataFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&KeepRecyclerTrackData));

        case MaxSingleAllocSizeInMBFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxSingleAllocSizeInMB));
        default:
            return NULL;
        }
    }


    void
    ConfigFlagsTable::VerboseDump()
    {

//  types:
//      String
//      Phases
//      Number
//      Boolean
//
// If the default value is not required it should be left empty
// For Phases, there is no default value. it should always be empty
// Default values for stings must be prefixed with 'L'. See AsmDumpMode
// Scroll till the extreme right to see the default values

#if DBG
if (IsEnabled(ArrayValidateFlag))
{
    Output::Print(u"-%s", u"ArrayValidate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArrayValidateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArrayValidateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArrayValidateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArrayValidateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MemOpMissingValueValidateFlag))
{
    Output::Print(u"-%s", u"MemOpMissingValueValidate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemOpMissingValueValidateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemOpMissingValueValidateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemOpMissingValueValidateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemOpMissingValueValidateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OOPJITFixupValidateFlag))
{
    Output::Print(u"-%s", u"OOPJITFixupValidate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OOPJITFixupValidateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OOPJITFixupValidateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OOPJITFixupValidateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OOPJITFixupValidateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef ARENA_MEMORY_VERIFY
if (IsEnabled(ArenaNoFreeListFlag))
{
    Output::Print(u"-%s", u"ArenaNoFreeList");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArenaNoFreeListFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArenaNoFreeListFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArenaNoFreeListFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArenaNoFreeListFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ArenaNoPageReuseFlag))
{
    Output::Print(u"-%s", u"ArenaNoPageReuse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArenaNoPageReuseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArenaNoPageReuseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArenaNoPageReuseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArenaNoPageReuseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ArenaUseHeapAllocFlag))
{
    Output::Print(u"-%s", u"ArenaUseHeapAlloc");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArenaUseHeapAllocFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArenaUseHeapAllocFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArenaUseHeapAllocFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArenaUseHeapAllocFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(ValidateInlineStackFlag))
{
    Output::Print(u"-%s", u"ValidateInlineStack");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ValidateInlineStackFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ValidateInlineStackFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ValidateInlineStackFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ValidateInlineStackFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmDiffFlag))
{
    Output::Print(u"-%s", u"AsmDiff");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmDiffFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmDiffFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmDiffFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmDiffFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmDumpModeFlag))
{
    Output::Print(u"-%s", u"AsmDumpMode");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmDumpModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmDumpModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmDumpModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmDumpModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmJsFlag))
{
    Output::Print(u"-%s", u"AsmJs");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmJsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmJsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmJsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmJsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmJsStopOnErrorFlag))
{
    Output::Print(u"-%s", u"AsmJsStopOnError");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmJsStopOnErrorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmJsStopOnErrorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmJsStopOnErrorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmJsStopOnErrorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmJsEdgeFlag))
{
    Output::Print(u"-%s", u"AsmJsEdge");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmJsEdgeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmJsEdgeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmJsEdgeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmJsEdgeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmFlag))
{
    Output::Print(u"-%s", u"Wasm");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmI64Flag))
{
    Output::Print(u"-%s", u"WasmI64");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmI64Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmI64Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmI64Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmI64Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmFastArrayFlag))
{
    Output::Print(u"-%s", u"WasmFastArray");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmFastArrayFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmFastArrayFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmFastArrayFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmFastArrayFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmSharedArrayVirtualBufferFlag))
{
    Output::Print(u"-%s", u"WasmSharedArrayVirtualBuffer");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmSharedArrayVirtualBufferFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmSharedArrayVirtualBufferFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmSharedArrayVirtualBufferFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmSharedArrayVirtualBufferFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmMathExFilterFlag))
{
    Output::Print(u"-%s", u"WasmMathExFilter");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmMathExFilterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmMathExFilterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmMathExFilterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmMathExFilterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmCheckVersionFlag))
{
    Output::Print(u"-%s", u"WasmCheckVersion");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmCheckVersionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmCheckVersionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmCheckVersionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmCheckVersionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmAssignModuleIDFlag))
{
    Output::Print(u"-%s", u"WasmAssignModuleID");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmAssignModuleIDFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmAssignModuleIDFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmAssignModuleIDFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmAssignModuleIDFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmIgnoreLimitsFlag))
{
    Output::Print(u"-%s", u"WasmIgnoreLimits");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmIgnoreLimitsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmIgnoreLimitsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmIgnoreLimitsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmIgnoreLimitsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmFoldFlag))
{
    Output::Print(u"-%s", u"WasmFold");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmFoldFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmFoldFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmFoldFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmFoldFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmIgnoreResponseFlag))
{
    Output::Print(u"-%s", u"WasmIgnoreResponse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmIgnoreResponseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmIgnoreResponseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmIgnoreResponseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmIgnoreResponseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmMaxTableSizeFlag))
{
    Output::Print(u"-%s", u"WasmMaxTableSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmMaxTableSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmMaxTableSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmMaxTableSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmMaxTableSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmThreadsFlag))
{
    Output::Print(u"-%s", u"WasmThreads");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmThreadsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmThreadsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmThreadsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmThreadsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmMultiValueFlag))
{
    Output::Print(u"-%s", u"WasmMultiValue");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmMultiValueFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmMultiValueFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmMultiValueFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmMultiValueFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmSignExtendsFlag))
{
    Output::Print(u"-%s", u"WasmSignExtends");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmSignExtendsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmSignExtendsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmSignExtendsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmSignExtendsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmNontrappingFlag))
{
    Output::Print(u"-%s", u"WasmNontrapping");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmNontrappingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmNontrappingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmNontrappingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmNontrappingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// WebAssembly Experimental Features
// Master WasmExperimental flag to activate WebAssembly experimental features
if (IsEnabled(WasmExperimentalFlag))
{
    Output::Print(u"-%s", u"WasmExperimental");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmExperimentalFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmExperimentalFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmExperimentalFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmExperimentalFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
if (IsEnabled(WasmSimdFlag))
{
    Output::Print(u"-%s", u"WasmSimd");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmSimdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmSimdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmSimdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmSimdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(AssertBreakFlag))
{
    Output::Print(u"-%s", u"AssertBreak");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AssertBreakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AssertBreakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AssertBreakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AssertBreakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AssertPopUpFlag))
{
    Output::Print(u"-%s", u"AssertPopUp");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AssertPopUpFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AssertPopUpFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AssertPopUpFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AssertPopUpFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AssertIgnoreFlag))
{
    Output::Print(u"-%s", u"AssertIgnore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AssertIgnoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AssertIgnoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AssertIgnoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AssertIgnoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsyncDebuggingFlag))
{
    Output::Print(u"-%s", u"AsyncDebugging");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsyncDebuggingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsyncDebuggingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsyncDebuggingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsyncDebuggingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOnNoProfileLimitFlag))
{
    Output::Print(u"-%s", u"BailOnNoProfileLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOnNoProfileLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOnNoProfileLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOnNoProfileLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOnNoProfileLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOnNoProfileRejitLimitFlag))
{
    Output::Print(u"-%s", u"BailOnNoProfileRejitLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOnNoProfileRejitLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOnNoProfileRejitLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOnNoProfileRejitLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOnNoProfileRejitLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BaselineModeFlag))
{
    Output::Print(u"-%s", u"BaselineMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BaselineModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BaselineModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BaselineModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BaselineModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpOnCrashFlag))
{
    Output::Print(u"-%s", u"DumpOnCrash");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpOnCrashFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpOnCrashFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpOnCrashFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpOnCrashFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FullMemoryDumpFlag))
{
    Output::Print(u"-%s", u"FullMemoryDump");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FullMemoryDumpFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FullMemoryDumpFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FullMemoryDumpFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FullMemoryDumpFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef BAILOUT_INJECTION
if (IsEnabled(BailOutFlag))
{
    Output::Print(u"-%s", u"BailOut");
    switch (FlagNumberPairSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOutAtEveryLineFlag))
{
    Output::Print(u"-%s", u"BailOutAtEveryLine");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutAtEveryLineFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutAtEveryLineFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutAtEveryLineFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutAtEveryLineFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOutAtEveryByteCodeFlag))
{
    Output::Print(u"-%s", u"BailOutAtEveryByteCode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutAtEveryByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutAtEveryByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutAtEveryByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutAtEveryByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOutAtEveryImplicitCallFlag))
{
    Output::Print(u"-%s", u"BailOutAtEveryImplicitCall");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutAtEveryImplicitCallFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutAtEveryImplicitCallFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutAtEveryImplicitCallFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutAtEveryImplicitCallFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOutByteCodeFlag))
{
    Output::Print(u"-%s", u"BailOutByteCode");
    switch (FlagNumberSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(BenchmarkFlag))
{
    Output::Print(u"-%s", u"Benchmark");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BenchmarkFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BenchmarkFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BenchmarkFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BenchmarkFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgJitFlag))
{
    Output::Print(u"-%s", u"BgJit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgJitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgJitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgJitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgJitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgParseFlag))
{
    Output::Print(u"-%s", u"BgParse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgJitDelayFlag))
{
    Output::Print(u"-%s", u"BgJitDelay");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgJitDelayFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgJitDelayFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgJitDelayFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgJitDelayFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgJitDelayFgBufferFlag))
{
    Output::Print(u"-%s", u"BgJitDelayFgBuffer");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgJitDelayFgBufferFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgJitDelayFgBufferFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgJitDelayFgBufferFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgJitDelayFgBufferFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgJitPendingFuncCapFlag))
{
    Output::Print(u"-%s", u"BgJitPendingFuncCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgJitPendingFuncCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgJitPendingFuncCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgJitPendingFuncCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgJitPendingFuncCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(CreateFunctionProxyFlag))
{
    Output::Print(u"-%s", u"CreateFunctionProxy");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CreateFunctionProxyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CreateFunctionProxyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CreateFunctionProxyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CreateFunctionProxyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(HybridFgJitFlag))
{
    Output::Print(u"-%s", u"HybridFgJit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(HybridFgJitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(HybridFgJitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(HybridFgJitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(HybridFgJitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(HybridFgJitBgQueueLengthThresholdFlag))
{
    Output::Print(u"-%s", u"HybridFgJitBgQueueLengthThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(HybridFgJitBgQueueLengthThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(HybridFgJitBgQueueLengthThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(HybridFgJitBgQueueLengthThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(HybridFgJitBgQueueLengthThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BytecodeHistFlag))
{
    Output::Print(u"-%s", u"BytecodeHist");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BytecodeHistFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BytecodeHistFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BytecodeHistFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BytecodeHistFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CurrentSourceInfoFlag))
{
    Output::Print(u"-%s", u"CurrentSourceInfo");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CurrentSourceInfoFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CurrentSourceInfoFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CurrentSourceInfoFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CurrentSourceInfoFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CFGLogFlag))
{
    Output::Print(u"-%s", u"CFGLog");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CFGLogFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CFGLogFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CFGLogFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CFGLogFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CheckAlignmentFlag))
{
    Output::Print(u"-%s", u"CheckAlignment");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CheckAlignmentFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CheckAlignmentFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CheckAlignmentFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CheckAlignmentFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CheckEmitBufferPermissionsFlag))
{
    Output::Print(u"-%s", u"CheckEmitBufferPermissions");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CheckEmitBufferPermissionsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CheckEmitBufferPermissionsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CheckEmitBufferPermissionsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CheckEmitBufferPermissionsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef CHECK_MEMORY_LEAK
if (IsEnabled(CheckMemoryLeakFlag))
{
    Output::Print(u"-%s", u"CheckMemoryLeak");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CheckMemoryLeakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CheckMemoryLeakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CheckMemoryLeakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CheckMemoryLeakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpOnLeakFlag))
{
    Output::Print(u"-%s", u"DumpOnLeak");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpOnLeakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpOnLeakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpOnLeakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpOnLeakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(CheckOpHelpersFlag))
{
    Output::Print(u"-%s", u"CheckOpHelpers");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CheckOpHelpersFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CheckOpHelpersFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CheckOpHelpersFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CheckOpHelpersFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CloneInlinedPolymorphicCachesFlag))
{
    Output::Print(u"-%s", u"CloneInlinedPolymorphicCaches");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CloneInlinedPolymorphicCachesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CloneInlinedPolymorphicCachesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CloneInlinedPolymorphicCachesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CloneInlinedPolymorphicCachesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConcurrentRuntimeFlag))
{
    Output::Print(u"-%s", u"ConcurrentRuntime");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConcurrentRuntimeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConcurrentRuntimeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConcurrentRuntimeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConcurrentRuntimeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConstructorInlineThresholdFlag))
{
    Output::Print(u"-%s", u"ConstructorInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConstructorInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConstructorInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConstructorInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConstructorInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConstructorCallsRequiredToFinalizeCachedTypeFlag))
{
    Output::Print(u"-%s", u"ConstructorCallsRequiredToFinalizeCachedType");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConstructorCallsRequiredToFinalizeCachedTypeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConstructorCallsRequiredToFinalizeCachedTypeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConstructorCallsRequiredToFinalizeCachedTypeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConstructorCallsRequiredToFinalizeCachedTypeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PropertyCacheMissPenaltyFlag))
{
    Output::Print(u"-%s", u"PropertyCacheMissPenalty");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PropertyCacheMissPenaltyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PropertyCacheMissPenaltyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PropertyCacheMissPenaltyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PropertyCacheMissPenaltyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PropertyCacheMissThresholdFlag))
{
    Output::Print(u"-%s", u"PropertyCacheMissThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PropertyCacheMissThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PropertyCacheMissThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PropertyCacheMissThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PropertyCacheMissThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PropertyCacheMissResetFlag))
{
    Output::Print(u"-%s", u"PropertyCacheMissReset");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PropertyCacheMissResetFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PropertyCacheMissResetFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PropertyCacheMissResetFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PropertyCacheMissResetFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DebugFlag))
{
    Output::Print(u"-%s", u"Debug");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DebugFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DebugFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DebugFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DebugFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DebugBreakFlag))
{
    Output::Print(u"-%s", u"DebugBreak");
    switch (FlagNumberSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DebugBreakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DebugBreakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DebugBreakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DebugBreakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(StatementDebugBreakFlag))
{
    Output::Print(u"-%s", u"StatementDebugBreak");
    switch (FlagNumberTrioSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StatementDebugBreakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StatementDebugBreakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StatementDebugBreakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StatementDebugBreakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DebugBreakOnPhaseBeginFlag))
{
    Output::Print(u"-%s", u"DebugBreakOnPhaseBegin");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DebugBreakOnPhaseBeginFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DebugBreakOnPhaseBeginFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DebugBreakOnPhaseBeginFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DebugBreakOnPhaseBeginFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(DebugWindowFlag))
{
    Output::Print(u"-%s", u"DebugWindow");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DebugWindowFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DebugWindowFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DebugWindowFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DebugWindowFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ParserStateCacheFlag))
{
    Output::Print(u"-%s", u"ParserStateCache");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ParserStateCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ParserStateCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ParserStateCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ParserStateCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CompressParserStateCacheFlag))
{
    Output::Print(u"-%s", u"CompressParserStateCache");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CompressParserStateCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CompressParserStateCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CompressParserStateCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CompressParserStateCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DeferTopLevelTillFirstCallFlag))
{
    Output::Print(u"-%s", u"DeferTopLevelTillFirstCall");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DeferTopLevelTillFirstCallFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DeferTopLevelTillFirstCallFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DeferTopLevelTillFirstCallFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DeferTopLevelTillFirstCallFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DeferParseFlag))
{
    Output::Print(u"-%s", u"DeferParse");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DeferParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DeferParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DeferParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DeferParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DirectCallTelemetryStatsFlag))
{
    Output::Print(u"-%s", u"DirectCallTelemetryStats");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DirectCallTelemetryStatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DirectCallTelemetryStatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DirectCallTelemetryStatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DirectCallTelemetryStatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisableArrayBTreeFlag))
{
    Output::Print(u"-%s", u"DisableArrayBTree");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisableArrayBTreeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisableArrayBTreeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisableArrayBTreeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisableArrayBTreeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisableRentalThreadingFlag))
{
    Output::Print(u"-%s", u"DisableRentalThreading");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisableRentalThreadingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisableRentalThreadingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisableRentalThreadingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisableRentalThreadingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisableVTuneSourceLineInfoFlag))
{
    Output::Print(u"-%s", u"DisableVTuneSourceLineInfo");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisableVTuneSourceLineInfoFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisableVTuneSourceLineInfoFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisableVTuneSourceLineInfoFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisableVTuneSourceLineInfoFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisplayMemStatsFlag))
{
    Output::Print(u"-%s", u"DisplayMemStats");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisplayMemStatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisplayMemStatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisplayMemStatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisplayMemStatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpFlag))
{
    Output::Print(u"-%s", u"Dump");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef DUMP_FRAGMENTATION_STATS
if (IsEnabled(DumpFragmentationStatsFlag))
{
    Output::Print(u"-%s", u"DumpFragmentationStats");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpFragmentationStatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpFragmentationStatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpFragmentationStatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpFragmentationStatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(DumpIRAddressesFlag))
{
    Output::Print(u"-%s", u"DumpIRAddresses");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpIRAddressesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpIRAddressesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpIRAddressesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpIRAddressesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpLineNoInColorFlag))
{
    Output::Print(u"-%s", u"DumpLineNoInColor");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpLineNoInColorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpLineNoInColorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpLineNoInColorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpLineNoInColorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
if (IsEnabled(DumpObjectGraphOnExitFlag))
{
    Output::Print(u"-%s", u"DumpObjectGraphOnExit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpObjectGraphOnExitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpObjectGraphOnExitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpObjectGraphOnExitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpObjectGraphOnExitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpObjectGraphOnCollectFlag))
{
    Output::Print(u"-%s", u"DumpObjectGraphOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpObjectGraphOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpObjectGraphOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpObjectGraphOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpObjectGraphOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(DumpEvalStringOnRemovalFlag))
{
    Output::Print(u"-%s", u"DumpEvalStringOnRemoval");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpEvalStringOnRemovalFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpEvalStringOnRemovalFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpEvalStringOnRemovalFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpEvalStringOnRemovalFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpObjectGraphOnEnumFlag))
{
    Output::Print(u"-%s", u"DumpObjectGraphOnEnum");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpObjectGraphOnEnumFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpObjectGraphOnEnumFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpObjectGraphOnEnumFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpObjectGraphOnEnumFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef DYNAMIC_PROFILE_STORAGE
if (IsEnabled(DynamicProfileCacheFlag))
{
    Output::Print(u"-%s", u"DynamicProfileCache");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DynamicProfileCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DynamicProfileCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DynamicProfileCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DynamicProfileCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DpcFlag))
{
    Output::Print(u"-%s", u"Dpc");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DpcFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DpcFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DpcFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DpcFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DynamicProfileCacheDirFlag))
{
    Output::Print(u"-%s", u"DynamicProfileCacheDir");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DynamicProfileCacheDirFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DynamicProfileCacheDirFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DynamicProfileCacheDirFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DynamicProfileCacheDirFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DynamicProfileInputFlag))
{
    Output::Print(u"-%s", u"DynamicProfileInput");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DynamicProfileInputFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DynamicProfileInputFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DynamicProfileInputFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DynamicProfileInputFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DpiFlag))
{
    Output::Print(u"-%s", u"Dpi");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DpiFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DpiFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DpiFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DpiFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef EDIT_AND_CONTINUE
if (IsEnabled(EditTestFlag))
{
    Output::Print(u"-%s", u"EditTest");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EditTestFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EditTestFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EditTestFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EditTestFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(WininetProfileCacheFlag))
{
    Output::Print(u"-%s", u"WininetProfileCache");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WininetProfileCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WininetProfileCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WininetProfileCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WininetProfileCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoDynamicProfileInMemoryCacheFlag))
{
    Output::Print(u"-%s", u"NoDynamicProfileInMemoryCache");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoDynamicProfileInMemoryCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoDynamicProfileInMemoryCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoDynamicProfileInMemoryCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoDynamicProfileInMemoryCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfileBasedSpeculativeJitFlag))
{
    Output::Print(u"-%s", u"ProfileBasedSpeculativeJit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileBasedSpeculativeJitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileBasedSpeculativeJitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileBasedSpeculativeJitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileBasedSpeculativeJitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfileBasedSpeculationCapFlag))
{
    Output::Print(u"-%s", u"ProfileBasedSpeculationCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileBasedSpeculationCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileBasedSpeculationCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileBasedSpeculationCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileBasedSpeculationCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag))
{
    Output::Print(u"-%s", u"ExecuteByteCodeBufferReturnsInvalidByteCode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ExpirableCollectionGCCountFlag))
{
    Output::Print(u"-%s", u"ExpirableCollectionGCCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExpirableCollectionGCCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExpirableCollectionGCCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExpirableCollectionGCCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExpirableCollectionGCCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ExpirableCollectionTriggerThresholdFlag))
{
    Output::Print(u"-%s", u"ExpirableCollectionTriggerThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExpirableCollectionTriggerThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExpirableCollectionTriggerThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExpirableCollectionTriggerThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExpirableCollectionTriggerThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(SkipSplitOnNoResultFlag))
{
    Output::Print(u"-%s", u"SkipSplitOnNoResult");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SkipSplitOnNoResultFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SkipSplitOnNoResultFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SkipSplitOnNoResultFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SkipSplitOnNoResultFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(Force32BitByteCodeFlag))
{
    Output::Print(u"-%s", u"Force32BitByteCode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(Force32BitByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(Force32BitByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(Force32BitByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(Force32BitByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(CollectGarbageFlag))
{
    Output::Print(u"-%s", u"CollectGarbage");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CollectGarbageFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CollectGarbageFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CollectGarbageFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CollectGarbageFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(IntlFlag))
{
    Output::Print(u"-%s", u"Intl");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IntlFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IntlFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IntlFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IntlFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(IntlBuiltInsFlag))
{
    Output::Print(u"-%s", u"IntlBuiltIns");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IntlBuiltInsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IntlBuiltInsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IntlBuiltInsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IntlBuiltInsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(IntlPlatformFlag))
{
    Output::Print(u"-%s", u"IntlPlatform");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IntlPlatformFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IntlPlatformFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IntlPlatformFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IntlPlatformFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(JsBuiltInFlag))
{
    Output::Print(u"-%s", u"JsBuiltIn");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JsBuiltInFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JsBuiltInFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JsBuiltInFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JsBuiltInFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(JitReproFlag))
{
    Output::Print(u"-%s", u"JitRepro");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JitReproFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JitReproFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JitReproFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JitReproFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EntryPointInfoRpcDataFlag))
{
    Output::Print(u"-%s", u"EntryPointInfoRpcData");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EntryPointInfoRpcDataFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EntryPointInfoRpcDataFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EntryPointInfoRpcDataFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EntryPointInfoRpcDataFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(LdChakraLibFlag))
{
    Output::Print(u"-%s", u"LdChakraLib");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LdChakraLibFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LdChakraLibFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LdChakraLibFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LdChakraLibFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TestChakraLibFlag))
{
    Output::Print(u"-%s", u"TestChakraLib");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TestChakraLibFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TestChakraLibFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TestChakraLibFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TestChakraLibFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
if (IsEnabled(ES6Flag))
{
    Output::Print(u"-%s", u"ES6");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// Master ES6 flag to enable ALL sub ES6 features/flags
if (IsEnabled(ES6AllFlag))
{
    Output::Print(u"-%s", u"ES6All");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6AllFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6AllFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6AllFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6AllFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// Master ES6 flag to enable Threshold ES6 features/flags
if (IsEnabled(ES6ExperimentalFlag))
{
    Output::Print(u"-%s", u"ES6Experimental");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ExperimentalFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ExperimentalFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ExperimentalFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ExperimentalFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// Per ES6 feature/flag

if (IsEnabled(ES7AsyncAwaitFlag))
{
    Output::Print(u"-%s", u"ES7AsyncAwait");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES7AsyncAwaitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES7AsyncAwaitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES7AsyncAwaitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES7AsyncAwaitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6DateParseFixFlag))
{
    Output::Print(u"-%s", u"ES6DateParseFix");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6DateParseFixFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6DateParseFixFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6DateParseFixFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6DateParseFixFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6FunctionNameFullFlag))
{
    Output::Print(u"-%s", u"ES6FunctionNameFull");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6FunctionNameFullFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6FunctionNameFullFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6FunctionNameFullFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6FunctionNameFullFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6GeneratorsFlag))
{
    Output::Print(u"-%s", u"ES6Generators");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6GeneratorsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6GeneratorsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6GeneratorsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6GeneratorsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES7ExponentiationOperatorFlag))
{
    Output::Print(u"-%s", u"ES7ExponentiationOperator");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES7ExponentiationOperatorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES7ExponentiationOperatorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES7ExponentiationOperatorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES7ExponentiationOperatorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ES7ValuesEntriesFlag))
{
    Output::Print(u"-%s", u"ES7ValuesEntries");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES7ValuesEntriesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES7ValuesEntriesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES7ValuesEntriesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES7ValuesEntriesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES7TrailingCommaFlag))
{
    Output::Print(u"-%s", u"ES7TrailingComma");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES7TrailingCommaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES7TrailingCommaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES7TrailingCommaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES7TrailingCommaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6IsConcatSpreadableFlag))
{
    Output::Print(u"-%s", u"ES6IsConcatSpreadable");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6IsConcatSpreadableFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6IsConcatSpreadableFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6IsConcatSpreadableFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6IsConcatSpreadableFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6MathFlag))
{
    Output::Print(u"-%s", u"ES6Math");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6MathFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6MathFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6MathFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6MathFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifndef COMPILE_DISABLE_ESDynamicImport
#define COMPILE_DISABLE_ESDynamicImport 0
#endif
if (IsEnabled(ESDynamicImportFlag))
{
    Output::Print(u"-%s", u"ESDynamicImport");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESDynamicImportFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESDynamicImportFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESDynamicImportFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESDynamicImportFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ES6ModuleFlag))
{
    Output::Print(u"-%s", u"ES6Module");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ModuleFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ModuleFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ModuleFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ModuleFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ObjectFlag))
{
    Output::Print(u"-%s", u"ES6Object");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ObjectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ObjectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ObjectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ObjectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6NumberFlag))
{
    Output::Print(u"-%s", u"ES6Number");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6NumberFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6NumberFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6NumberFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6NumberFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ObjectLiteralsFlag))
{
    Output::Print(u"-%s", u"ES6ObjectLiterals");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ObjectLiteralsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ObjectLiteralsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ObjectLiteralsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ObjectLiteralsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ProxyFlag))
{
    Output::Print(u"-%s", u"ES6Proxy");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ProxyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ProxyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ProxyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ProxyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6RestFlag))
{
    Output::Print(u"-%s", u"ES6Rest");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6RestFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6RestFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6RestFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6RestFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6SpreadFlag))
{
    Output::Print(u"-%s", u"ES6Spread");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6SpreadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6SpreadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6SpreadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6SpreadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6StringFlag))
{
    Output::Print(u"-%s", u"ES6String");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6StringFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6StringFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6StringFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6StringFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6StringPrototypeFixesFlag))
{
    Output::Print(u"-%s", u"ES6StringPrototypeFixes");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6StringPrototypeFixesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6StringPrototypeFixesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6StringPrototypeFixesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6StringPrototypeFixesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES2018ObjectRestSpreadFlag))
{
    Output::Print(u"-%s", u"ES2018ObjectRestSpread");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES2018ObjectRestSpreadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES2018ObjectRestSpreadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES2018ObjectRestSpreadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES2018ObjectRestSpreadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ES6PrototypeChainFlag))
{
    Output::Print(u"-%s", u"ES6PrototypeChain");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6PrototypeChainFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6PrototypeChainFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6PrototypeChainFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6PrototypeChainFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ToPrimitiveFlag))
{
    Output::Print(u"-%s", u"ES6ToPrimitive");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ToPrimitiveFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ToPrimitiveFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ToPrimitiveFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ToPrimitiveFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ToLengthFlag))
{
    Output::Print(u"-%s", u"ES6ToLength");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ToLengthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ToLengthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ToLengthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ToLengthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ToStringTagFlag))
{
    Output::Print(u"-%s", u"ES6ToStringTag");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ToStringTagFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ToStringTagFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ToStringTagFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ToStringTagFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6UnicodeFlag))
{
    Output::Print(u"-%s", u"ES6Unicode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6UnicodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6UnicodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6UnicodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6UnicodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6UnicodeVerboseFlag))
{
    Output::Print(u"-%s", u"ES6UnicodeVerbose");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6UnicodeVerboseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6UnicodeVerboseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6UnicodeVerboseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6UnicodeVerboseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6UnscopablesFlag))
{
    Output::Print(u"-%s", u"ES6Unscopables");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6UnscopablesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6UnscopablesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6UnscopablesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6UnscopablesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6RegExStickyFlag))
{
    Output::Print(u"-%s", u"ES6RegExSticky");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6RegExStickyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6RegExStickyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6RegExStickyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6RegExStickyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES2018RegExDotAllFlag))
{
    Output::Print(u"-%s", u"ES2018RegExDotAll");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES2018RegExDotAllFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES2018RegExDotAllFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES2018RegExDotAllFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES2018RegExDotAllFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ESExportNsAsFlag))
{
    Output::Print(u"-%s", u"ESExportNsAs");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESExportNsAsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESExportNsAsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESExportNsAsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESExportNsAsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES2018AsyncIterationFlag))
{
    Output::Print(u"-%s", u"ES2018AsyncIteration");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES2018AsyncIterationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES2018AsyncIterationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES2018AsyncIterationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES2018AsyncIterationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ESTopLevelAwaitFlag))
{
    Output::Print(u"-%s", u"ESTopLevelAwait");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESTopLevelAwaitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESTopLevelAwaitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESTopLevelAwaitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESTopLevelAwaitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
    #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
#endif
if (IsEnabled(ES6RegExPrototypePropertiesFlag))
{
    Output::Print(u"-%s", u"ES6RegExPrototypeProperties");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6RegExPrototypePropertiesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6RegExPrototypePropertiesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6RegExPrototypePropertiesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6RegExPrototypePropertiesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifndef COMPILE_DISABLE_ES6RegExSymbols
    #define COMPILE_DISABLE_ES6RegExSymbols 0
#endif

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
if (IsEnabled(ES6RegExSymbolsFlag))
{
    Output::Print(u"-%s", u"ES6RegExSymbols");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6RegExSymbolsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6RegExSymbolsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6RegExSymbolsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6RegExSymbolsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ES6VerboseFlag))
{
    Output::Print(u"-%s", u"ES6Verbose");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6VerboseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6VerboseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6VerboseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6VerboseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ESObjectGetOwnPropertyDescriptorsFlag))
{
    Output::Print(u"-%s", u"ESObjectGetOwnPropertyDescriptors");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESObjectGetOwnPropertyDescriptorsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESObjectGetOwnPropertyDescriptorsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESObjectGetOwnPropertyDescriptorsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESObjectGetOwnPropertyDescriptorsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifndef COMPILE_DISABLE_ESSharedArrayBuffer
    #define COMPILE_DISABLE_ESSharedArrayBuffer 0
#endif

if (IsEnabled(ESSharedArrayBufferFlag))
{
    Output::Print(u"-%s", u"ESSharedArrayBuffer");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESSharedArrayBufferFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESSharedArrayBufferFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESSharedArrayBufferFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESSharedArrayBufferFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// Newer language feature flags

// ES BigInt flag
if (IsEnabled(ESBigIntFlag))
{
    Output::Print(u"-%s", u"ESBigInt");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESBigIntFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESBigIntFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESBigIntFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESBigIntFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Numeric Separator support for numeric constants
if (IsEnabled(ESNumericSeparatorFlag))
{
    Output::Print(u"-%s", u"ESNumericSeparator");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESNumericSeparatorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESNumericSeparatorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESNumericSeparatorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESNumericSeparatorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Nullish coalescing operator support (??)
if (IsEnabled(ESNullishCoalescingOperatorFlag))
{
    Output::Print(u"-%s", u"ESNullishCoalescingOperator");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESNullishCoalescingOperatorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESNullishCoalescingOperatorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESNullishCoalescingOperatorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESNullishCoalescingOperatorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Hashbang support for interpreter directive syntax
if (IsEnabled(ESHashbangFlag))
{
    Output::Print(u"-%s", u"ESHashbang");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESHashbangFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESHashbangFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESHashbangFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESHashbangFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Symbol.prototype.description flag
if (IsEnabled(ESSymbolDescriptionFlag))
{
    Output::Print(u"-%s", u"ESSymbolDescription");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESSymbolDescriptionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESSymbolDescriptionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESSymbolDescriptionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESSymbolDescriptionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ESArrayFindFromLastFlag))
{
    Output::Print(u"-%s", u"ESArrayFindFromLast");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESArrayFindFromLastFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESArrayFindFromLastFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESArrayFindFromLastFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESArrayFindFromLastFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Promise.any and AggregateError flag
if (IsEnabled(ESPromiseAnyFlag))
{
    Output::Print(u"-%s", u"ESPromiseAny");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESPromiseAnyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESPromiseAnyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESPromiseAnyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESPromiseAnyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES import.meta keyword meta-property
if (IsEnabled(ESImportMetaFlag))
{
    Output::Print(u"-%s", u"ESImportMeta");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESImportMetaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESImportMetaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESImportMetaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESImportMetaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

//ES globalThis flag
if (IsEnabled(ESGlobalThisFlag))
{
    Output::Print(u"-%s", u"ESGlobalThis");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESGlobalThisFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESGlobalThisFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESGlobalThisFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESGlobalThisFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// This flag to be removed once JITing generator functions is stable
if (IsEnabled(JitES6GeneratorsFlag))
{
    Output::Print(u"-%s", u"JitES6Generators");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JitES6GeneratorsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JitES6GeneratorsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JitES6GeneratorsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JitES6GeneratorsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(FastLineColumnCalculationFlag))
{
    Output::Print(u"-%s", u"FastLineColumnCalculation");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FastLineColumnCalculationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FastLineColumnCalculationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FastLineColumnCalculationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FastLineColumnCalculationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FilenameFlag))
{
    Output::Print(u"-%s", u"Filename");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FilenameFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FilenameFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FilenameFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FilenameFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FreeRejittedCodeFlag))
{
    Output::Print(u"-%s", u"FreeRejittedCode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FreeRejittedCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FreeRejittedCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FreeRejittedCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FreeRejittedCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceGuardPagesFlag))
{
    Output::Print(u"-%s", u"ForceGuardPages");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceGuardPagesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceGuardPagesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceGuardPagesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceGuardPagesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PrintGuardPageBoundsFlag))
{
    Output::Print(u"-%s", u"PrintGuardPageBounds");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintGuardPageBoundsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintGuardPageBoundsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintGuardPageBoundsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintGuardPageBoundsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceLegacyEngineFlag))
{
    Output::Print(u"-%s", u"ForceLegacyEngine");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceLegacyEngineFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceLegacyEngineFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceLegacyEngineFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceLegacyEngineFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceFlag))
{
    Output::Print(u"-%s", u"Force");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(StressFlag))
{
    Output::Print(u"-%s", u"Stress");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceArrayBTreeFlag))
{
    Output::Print(u"-%s", u"ForceArrayBTree");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceArrayBTreeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceArrayBTreeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceArrayBTreeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceArrayBTreeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(StrongArraySortFlag))
{
    Output::Print(u"-%s", u"StrongArraySort");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StrongArraySortFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StrongArraySortFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StrongArraySortFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StrongArraySortFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceCleanPropertyOnCollectFlag))
{
    Output::Print(u"-%s", u"ForceCleanPropertyOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceCleanPropertyOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceCleanPropertyOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceCleanPropertyOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceCleanPropertyOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceCleanCacheOnCollectFlag))
{
    Output::Print(u"-%s", u"ForceCleanCacheOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceCleanCacheOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceCleanCacheOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceCleanCacheOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceCleanCacheOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceGCAfterJSONParseFlag))
{
    Output::Print(u"-%s", u"ForceGCAfterJSONParse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceGCAfterJSONParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceGCAfterJSONParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceGCAfterJSONParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceGCAfterJSONParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceDecommitOnCollectFlag))
{
    Output::Print(u"-%s", u"ForceDecommitOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceDecommitOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceDecommitOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceDecommitOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceDecommitOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceDeferParseFlag))
{
    Output::Print(u"-%s", u"ForceDeferParse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceDeferParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceDeferParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceDeferParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceDeferParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceDiagnosticsModeFlag))
{
    Output::Print(u"-%s", u"ForceDiagnosticsMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceDiagnosticsModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceDiagnosticsModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceDiagnosticsModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceDiagnosticsModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceGetWriteWatchOOMFlag))
{
    Output::Print(u"-%s", u"ForceGetWriteWatchOOM");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceGetWriteWatchOOMFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceGetWriteWatchOOMFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceGetWriteWatchOOMFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceGetWriteWatchOOMFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForcePostLowerGlobOptInstrStringFlag))
{
    Output::Print(u"-%s", u"ForcePostLowerGlobOptInstrString");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForcePostLowerGlobOptInstrStringFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForcePostLowerGlobOptInstrStringFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForcePostLowerGlobOptInstrStringFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForcePostLowerGlobOptInstrStringFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSplitScopeFlag))
{
    Output::Print(u"-%s", u"ForceSplitScope");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSplitScopeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSplitScopeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSplitScopeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSplitScopeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnumerateSpecialPropertiesInDebuggerFlag))
{
    Output::Print(u"-%s", u"EnumerateSpecialPropertiesInDebugger");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnumerateSpecialPropertiesInDebuggerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnumerateSpecialPropertiesInDebuggerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnumerateSpecialPropertiesInDebuggerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnumerateSpecialPropertiesInDebuggerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableContinueAfterExceptionWrappersForHelpersFlag))
{
    Output::Print(u"-%s", u"EnableContinueAfterExceptionWrappersForHelpers");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableContinueAfterExceptionWrappersForHelpersFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableContinueAfterExceptionWrappersForHelpersFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableContinueAfterExceptionWrappersForHelpersFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableContinueAfterExceptionWrappersForHelpersFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableContinueAfterExceptionWrappersForBuiltInsFlag))
{
    Output::Print(u"-%s", u"EnableContinueAfterExceptionWrappersForBuiltIns");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableContinueAfterExceptionWrappersForBuiltInsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableContinueAfterExceptionWrappersForBuiltInsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableContinueAfterExceptionWrappersForBuiltInsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableContinueAfterExceptionWrappersForBuiltInsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableFunctionSourceReportForHeapEnumFlag))
{
    Output::Print(u"-%s", u"EnableFunctionSourceReportForHeapEnum");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableFunctionSourceReportForHeapEnumFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableFunctionSourceReportForHeapEnumFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableFunctionSourceReportForHeapEnumFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableFunctionSourceReportForHeapEnumFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceFragmentAddressSpaceFlag))
{
    Output::Print(u"-%s", u"ForceFragmentAddressSpace");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceFragmentAddressSpaceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceFragmentAddressSpaceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceFragmentAddressSpaceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceFragmentAddressSpaceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceOOMOnEBCommitFlag))
{
    Output::Print(u"-%s", u"ForceOOMOnEBCommit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceOOMOnEBCommitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceOOMOnEBCommitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceOOMOnEBCommitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceOOMOnEBCommitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceDynamicProfileFlag))
{
    Output::Print(u"-%s", u"ForceDynamicProfile");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceDynamicProfileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceDynamicProfileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceDynamicProfileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceDynamicProfileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceES5ArrayFlag))
{
    Output::Print(u"-%s", u"ForceES5Array");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceES5ArrayFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceES5ArrayFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceES5ArrayFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceES5ArrayFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceAsmJsLinkFailFlag))
{
    Output::Print(u"-%s", u"ForceAsmJsLinkFail");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceAsmJsLinkFailFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceAsmJsLinkFailFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceAsmJsLinkFailFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceAsmJsLinkFailFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceExpireOnNonCacheCollectFlag))
{
    Output::Print(u"-%s", u"ForceExpireOnNonCacheCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceExpireOnNonCacheCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceExpireOnNonCacheCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceExpireOnNonCacheCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceExpireOnNonCacheCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceFastPathFlag))
{
    Output::Print(u"-%s", u"ForceFastPath");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceFastPathFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceFastPathFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceFastPathFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceFastPathFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceFloatPrefFlag))
{
    Output::Print(u"-%s", u"ForceFloatPref");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceFloatPrefFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceFloatPrefFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceFloatPrefFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceFloatPrefFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceJITLoopBodyFlag))
{
    Output::Print(u"-%s", u"ForceJITLoopBody");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceJITLoopBodyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceJITLoopBodyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceJITLoopBodyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceJITLoopBodyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceStaticInterpreterThunkFlag))
{
    Output::Print(u"-%s", u"ForceStaticInterpreterThunk");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceStaticInterpreterThunkFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceStaticInterpreterThunkFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceStaticInterpreterThunkFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceStaticInterpreterThunkFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpCommentsFromReferencedFilesFlag))
{
    Output::Print(u"-%s", u"DumpCommentsFromReferencedFiles");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpCommentsFromReferencedFilesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpCommentsFromReferencedFilesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpCommentsFromReferencedFilesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpCommentsFromReferencedFilesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DelayFullJITSmallFuncFlag))
{
    Output::Print(u"-%s", u"DelayFullJITSmallFunc");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DelayFullJITSmallFuncFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DelayFullJITSmallFuncFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DelayFullJITSmallFuncFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DelayFullJITSmallFuncFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableFatalErrorOnOOMFlag))
{
    Output::Print(u"-%s", u"EnableFatalErrorOnOOM");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableFatalErrorOnOOMFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableFatalErrorOnOOMFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableFatalErrorOnOOMFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableFatalErrorOnOOMFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if defined(_M_ARM32_OR_ARM64)
if (IsEnabled(ForceLocalsPtrFlag))
{
    Output::Print(u"-%s", u"ForceLocalsPtr");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceLocalsPtrFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceLocalsPtrFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceLocalsPtrFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceLocalsPtrFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(DeferLoadingAvailableSourceFlag))
{
    Output::Print(u"-%s", u"DeferLoadingAvailableSource");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DeferLoadingAvailableSourceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DeferLoadingAvailableSourceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DeferLoadingAvailableSourceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DeferLoadingAvailableSourceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceNativeFlag))
{
    Output::Print(u"-%s", u"ForceNative");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceNativeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceNativeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceNativeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceNativeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSerializedFlag))
{
    Output::Print(u"-%s", u"ForceSerialized");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSerializedFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSerializedFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSerializedFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSerializedFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSerializedBytecodeMajorVersionFlag))
{
    Output::Print(u"-%s", u"ForceSerializedBytecodeMajorVersion");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSerializedBytecodeMajorVersionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSerializedBytecodeMajorVersionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSerializedBytecodeMajorVersionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSerializedBytecodeMajorVersionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSerializedBytecodeVersionSchemaFlag))
{
    Output::Print(u"-%s", u"ForceSerializedBytecodeVersionSchema");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSerializedBytecodeVersionSchemaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSerializedBytecodeVersionSchemaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSerializedBytecodeVersionSchemaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSerializedBytecodeVersionSchemaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceStrictModeFlag))
{
    Output::Print(u"-%s", u"ForceStrictMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceStrictModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceStrictModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceStrictModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceStrictModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceUndoDeferFlag))
{
    Output::Print(u"-%s", u"ForceUndoDefer");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceUndoDeferFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceUndoDeferFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceUndoDeferFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceUndoDeferFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceBlockingConcurrentCollectFlag))
{
    Output::Print(u"-%s", u"ForceBlockingConcurrentCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceBlockingConcurrentCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceBlockingConcurrentCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceBlockingConcurrentCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceBlockingConcurrentCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FreTestDiagModeFlag))
{
    Output::Print(u"-%s", u"FreTestDiagMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FreTestDiagModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FreTestDiagModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FreTestDiagModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FreTestDiagModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef BYTECODE_TESTING
if (IsEnabled(ByteCodeBranchLimitFlag))
{
    Output::Print(u"-%s", u"ByteCodeBranchLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ByteCodeBranchLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ByteCodeBranchLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ByteCodeBranchLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ByteCodeBranchLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MediumByteCodeLayoutFlag))
{
    Output::Print(u"-%s", u"MediumByteCodeLayout");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MediumByteCodeLayoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MediumByteCodeLayoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MediumByteCodeLayoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MediumByteCodeLayoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LargeByteCodeLayoutFlag))
{
    Output::Print(u"-%s", u"LargeByteCodeLayout");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LargeByteCodeLayoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LargeByteCodeLayoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LargeByteCodeLayoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LargeByteCodeLayoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(InduceCodeGenFailureFlag))
{
    Output::Print(u"-%s", u"InduceCodeGenFailure");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InduceCodeGenFailureFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InduceCodeGenFailureFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InduceCodeGenFailureFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InduceCodeGenFailureFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InduceCodeGenFailureSeedFlag))
{
    Output::Print(u"-%s", u"InduceCodeGenFailureSeed");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InduceCodeGenFailureSeedFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InduceCodeGenFailureSeedFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InduceCodeGenFailureSeedFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InduceCodeGenFailureSeedFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InjectPartiallyInitializedInterpreterFrameErrorFlag))
{
    Output::Print(u"-%s", u"InjectPartiallyInitializedInterpreterFrameError");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InjectPartiallyInitializedInterpreterFrameErrorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InjectPartiallyInitializedInterpreterFrameErrorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InjectPartiallyInitializedInterpreterFrameErrorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InjectPartiallyInitializedInterpreterFrameErrorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag))
{
    Output::Print(u"-%s", u"InjectPartiallyInitializedInterpreterFrameErrorType");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(GenerateByteCodeBufferReturnsCantGenerateFlag))
{
    Output::Print(u"-%s", u"GenerateByteCodeBufferReturnsCantGenerate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(GenerateByteCodeBufferReturnsCantGenerateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(GenerateByteCodeBufferReturnsCantGenerateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(GenerateByteCodeBufferReturnsCantGenerateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(GenerateByteCodeBufferReturnsCantGenerateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(GoptCleanupThresholdFlag))
{
    Output::Print(u"-%s", u"GoptCleanupThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(GoptCleanupThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(GoptCleanupThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(GoptCleanupThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(GoptCleanupThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmGoptCleanupThresholdFlag))
{
    Output::Print(u"-%s", u"AsmGoptCleanupThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmGoptCleanupThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmGoptCleanupThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmGoptCleanupThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmGoptCleanupThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(HighPrecisionDateFlag))
{
    Output::Print(u"-%s", u"HighPrecisionDate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(HighPrecisionDateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(HighPrecisionDateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(HighPrecisionDateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(HighPrecisionDateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineCountMaxFlag))
{
    Output::Print(u"-%s", u"InlineCountMax");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineCountMaxFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineCountMaxFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineCountMaxFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineCountMaxFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineCountMaxInLoopBodiesFlag))
{
    Output::Print(u"-%s", u"InlineCountMaxInLoopBodies");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineCountMaxInLoopBodiesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineCountMaxInLoopBodiesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineCountMaxInLoopBodiesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineCountMaxInLoopBodiesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(icminlbFlag))
{
    Output::Print(u"-%s", u"icminlb");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(icminlbFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(icminlbFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(icminlbFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(icminlbFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineInLoopBodyScaleDownFactorFlag))
{
    Output::Print(u"-%s", u"InlineInLoopBodyScaleDownFactor");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineInLoopBodyScaleDownFactorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineInLoopBodyScaleDownFactorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineInLoopBodyScaleDownFactorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineInLoopBodyScaleDownFactorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(iilbsdfFlag))
{
    Output::Print(u"-%s", u"iilbsdf");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(iilbsdfFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(iilbsdfFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(iilbsdfFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(iilbsdfFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineThresholdFlag))
{
    Output::Print(u"-%s", u"InlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AggressiveInlineCountMaxFlag))
{
    Output::Print(u"-%s", u"AggressiveInlineCountMax");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AggressiveInlineCountMaxFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AggressiveInlineCountMaxFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AggressiveInlineCountMaxFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AggressiveInlineCountMaxFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AggressiveInlineThresholdFlag))
{
    Output::Print(u"-%s", u"AggressiveInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AggressiveInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AggressiveInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AggressiveInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AggressiveInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineThresholdAdjustCountInLargeFunctionFlag))
{
    Output::Print(u"-%s", u"InlineThresholdAdjustCountInLargeFunction");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineThresholdAdjustCountInLargeFunctionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineThresholdAdjustCountInLargeFunctionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineThresholdAdjustCountInLargeFunctionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineThresholdAdjustCountInLargeFunctionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineThresholdAdjustCountInMediumSizedFunctionFlag))
{
    Output::Print(u"-%s", u"InlineThresholdAdjustCountInMediumSizedFunction");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineThresholdAdjustCountInMediumSizedFunctionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineThresholdAdjustCountInMediumSizedFunctionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineThresholdAdjustCountInMediumSizedFunctionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineThresholdAdjustCountInMediumSizedFunctionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineThresholdAdjustCountInSmallFunctionFlag))
{
    Output::Print(u"-%s", u"InlineThresholdAdjustCountInSmallFunction");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineThresholdAdjustCountInSmallFunctionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineThresholdAdjustCountInSmallFunctionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineThresholdAdjustCountInSmallFunctionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineThresholdAdjustCountInSmallFunctionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmJsInlineAdjustFlag))
{
    Output::Print(u"-%s", u"AsmJsInlineAdjust");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmJsInlineAdjustFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmJsInlineAdjustFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmJsInlineAdjustFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmJsInlineAdjustFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InterpretFlag))
{
    Output::Print(u"-%s", u"Interpret");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InterpretFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InterpretFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InterpretFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InterpretFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InstrumentFlag))
{
    Output::Print(u"-%s", u"Instrument");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InstrumentFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InstrumentFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InstrumentFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InstrumentFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(JitQueueThresholdFlag))
{
    Output::Print(u"-%s", u"JitQueueThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JitQueueThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JitQueueThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JitQueueThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JitQueueThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef LEAK_REPORT
if (IsEnabled(LeakReportFlag))
{
    Output::Print(u"-%s", u"LeakReport");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LeakReportFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LeakReportFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LeakReportFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LeakReportFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(LoopInlineThresholdFlag))
{
    Output::Print(u"-%s", u"LoopInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LeafInlineThresholdFlag))
{
    Output::Print(u"-%s", u"LeafInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LeafInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LeafInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LeafInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LeafInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConstantArgumentInlineThresholdFlag))
{
    Output::Print(u"-%s", u"ConstantArgumentInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConstantArgumentInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConstantArgumentInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConstantArgumentInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConstantArgumentInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecursiveInlineThresholdFlag))
{
    Output::Print(u"-%s", u"RecursiveInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecursiveInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecursiveInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecursiveInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecursiveInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecursiveInlineDepthMaxFlag))
{
    Output::Print(u"-%s", u"RecursiveInlineDepthMax");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecursiveInlineDepthMaxFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecursiveInlineDepthMaxFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecursiveInlineDepthMaxFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecursiveInlineDepthMaxFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecursiveInlineDepthMinFlag))
{
    Output::Print(u"-%s", u"RecursiveInlineDepthMin");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecursiveInlineDepthMinFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecursiveInlineDepthMinFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecursiveInlineDepthMinFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecursiveInlineDepthMinFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RedeferralCapFlag))
{
    Output::Print(u"-%s", u"RedeferralCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RedeferralCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RedeferralCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RedeferralCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RedeferralCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopFlag))
{
    Output::Print(u"-%s", u"Loop");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopInterpretCountFlag))
{
    Output::Print(u"-%s", u"LoopInterpretCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopInterpretCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopInterpretCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopInterpretCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopInterpretCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(licFlag))
{
    Output::Print(u"-%s", u"lic");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(licFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(licFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(licFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(licFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopProfileIterationsFlag))
{
    Output::Print(u"-%s", u"LoopProfileIterations");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopProfileIterationsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopProfileIterationsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopProfileIterationsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopProfileIterationsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OutsideLoopInlineThresholdFlag))
{
    Output::Print(u"-%s", u"OutsideLoopInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OutsideLoopInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OutsideLoopInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OutsideLoopInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OutsideLoopInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxFuncInlineDepthFlag))
{
    Output::Print(u"-%s", u"MaxFuncInlineDepth");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxFuncInlineDepthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxFuncInlineDepthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxFuncInlineDepthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxFuncInlineDepthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxNumberOfInlineesWithLoopFlag))
{
    Output::Print(u"-%s", u"MaxNumberOfInlineesWithLoop");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxNumberOfInlineesWithLoopFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxNumberOfInlineesWithLoopFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxNumberOfInlineesWithLoopFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxNumberOfInlineesWithLoopFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef MEMSPECT_TRACKING
if (IsEnabled(MemspectFlag))
{
    Output::Print(u"-%s", u"Memspect");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemspectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemspectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemspectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemspectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(PolymorphicInlineThresholdFlag))
{
    Output::Print(u"-%s", u"PolymorphicInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PolymorphicInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PolymorphicInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PolymorphicInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PolymorphicInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PrimeRecyclerFlag))
{
    Output::Print(u"-%s", u"PrimeRecycler");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrimeRecyclerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrimeRecyclerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrimeRecyclerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrimeRecyclerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceEngineRefcountFlag))
{
    Output::Print(u"-%s", u"TraceEngineRefcount");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceEngineRefcountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceEngineRefcountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceEngineRefcountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceEngineRefcountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
if (IsEnabled(LeakStackTraceFlag))
{
    Output::Print(u"-%s", u"LeakStackTrace");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LeakStackTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LeakStackTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LeakStackTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LeakStackTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceMemoryLeakFlag))
{
    Output::Print(u"-%s", u"ForceMemoryLeak");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceMemoryLeakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceMemoryLeakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceMemoryLeakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceMemoryLeakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(DumpAfterFinalGCFlag))
{
    Output::Print(u"-%s", u"DumpAfterFinalGC");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpAfterFinalGCFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpAfterFinalGCFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpAfterFinalGCFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpAfterFinalGCFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceOldDateAPIFlag))
{
    Output::Print(u"-%s", u"ForceOldDateAPI");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceOldDateAPIFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceOldDateAPIFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceOldDateAPIFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceOldDateAPIFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(JitLoopBodyHotLoopThresholdFlag))
{
    Output::Print(u"-%s", u"JitLoopBodyHotLoopThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JitLoopBodyHotLoopThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JitLoopBodyHotLoopThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JitLoopBodyHotLoopThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JitLoopBodyHotLoopThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopBodySizeThresholdToDisableOptsFlag))
{
    Output::Print(u"-%s", u"LoopBodySizeThresholdToDisableOpts");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopBodySizeThresholdToDisableOptsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopBodySizeThresholdToDisableOptsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopBodySizeThresholdToDisableOptsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopBodySizeThresholdToDisableOptsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MaxJitThreadCountFlag))
{
    Output::Print(u"-%s", u"MaxJitThreadCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxJitThreadCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxJitThreadCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxJitThreadCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxJitThreadCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceMaxJitThreadCountFlag))
{
    Output::Print(u"-%s", u"ForceMaxJitThreadCount");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceMaxJitThreadCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceMaxJitThreadCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceMaxJitThreadCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceMaxJitThreadCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MitigateSpectreFlag))
{
    Output::Print(u"-%s", u"MitigateSpectre");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MitigateSpectreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MitigateSpectreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MitigateSpectreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MitigateSpectreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(AddMaskingBlocksFlag))
{
    Output::Print(u"-%s", u"AddMaskingBlocks");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AddMaskingBlocksFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AddMaskingBlocksFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AddMaskingBlocksFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AddMaskingBlocksFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(PoisonVarArrayLoadFlag))
{
    Output::Print(u"-%s", u"PoisonVarArrayLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonVarArrayLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonVarArrayLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonVarArrayLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonVarArrayLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonIntArrayLoadFlag))
{
    Output::Print(u"-%s", u"PoisonIntArrayLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonIntArrayLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonIntArrayLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonIntArrayLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonIntArrayLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonFloatArrayLoadFlag))
{
    Output::Print(u"-%s", u"PoisonFloatArrayLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonFloatArrayLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonFloatArrayLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonFloatArrayLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonFloatArrayLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonTypedArrayLoadFlag))
{
    Output::Print(u"-%s", u"PoisonTypedArrayLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonTypedArrayLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonTypedArrayLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonTypedArrayLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonTypedArrayLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonStringLoadFlag))
{
    Output::Print(u"-%s", u"PoisonStringLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonStringLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonStringLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonStringLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonStringLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonObjectsForLoadsFlag))
{
    Output::Print(u"-%s", u"PoisonObjectsForLoads");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonObjectsForLoadsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonObjectsForLoadsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonObjectsForLoadsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonObjectsForLoadsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(PoisonVarArrayStoreFlag))
{
    Output::Print(u"-%s", u"PoisonVarArrayStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonVarArrayStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonVarArrayStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonVarArrayStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonVarArrayStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonIntArrayStoreFlag))
{
    Output::Print(u"-%s", u"PoisonIntArrayStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonIntArrayStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonIntArrayStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonIntArrayStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonIntArrayStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonFloatArrayStoreFlag))
{
    Output::Print(u"-%s", u"PoisonFloatArrayStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonFloatArrayStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonFloatArrayStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonFloatArrayStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonFloatArrayStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonTypedArrayStoreFlag))
{
    Output::Print(u"-%s", u"PoisonTypedArrayStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonTypedArrayStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonTypedArrayStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonTypedArrayStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonTypedArrayStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonStringStoreFlag))
{
    Output::Print(u"-%s", u"PoisonStringStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonStringStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonStringStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonStringStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonStringStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonObjectsForStoresFlag))
{
    Output::Print(u"-%s", u"PoisonObjectsForStores");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonObjectsForStoresFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonObjectsForStoresFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonObjectsForStoresFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonObjectsForStoresFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MinInterpretCountFlag))
{
    Output::Print(u"-%s", u"MinInterpretCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinInterpretCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinInterpretCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinInterpretCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinInterpretCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinSimpleJitRunCountFlag))
{
    Output::Print(u"-%s", u"MinSimpleJitRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinSimpleJitRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinSimpleJitRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinSimpleJitRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinSimpleJitRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxInterpretCountFlag))
{
    Output::Print(u"-%s", u"MaxInterpretCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxInterpretCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxInterpretCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxInterpretCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxInterpretCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MicFlag))
{
    Output::Print(u"-%s", u"Mic");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MicFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MicFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MicFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MicFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxSimpleJitRunCountFlag))
{
    Output::Print(u"-%s", u"MaxSimpleJitRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxSimpleJitRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxSimpleJitRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxSimpleJitRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxSimpleJitRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MsjrcFlag))
{
    Output::Print(u"-%s", u"Msjrc");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MsjrcFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MsjrcFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MsjrcFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MsjrcFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinMemOpCountFlag))
{
    Output::Print(u"-%s", u"MinMemOpCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinMemOpCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinMemOpCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinMemOpCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinMemOpCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MmocFlag))
{
    Output::Print(u"-%s", u"Mmoc");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MmocFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MmocFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MmocFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MmocFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if ENABLE_COPYONACCESS_ARRAY
if (IsEnabled(MaxCopyOnAccessArrayLengthFlag))
{
    Output::Print(u"-%s", u"MaxCopyOnAccessArrayLength");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxCopyOnAccessArrayLengthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxCopyOnAccessArrayLengthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxCopyOnAccessArrayLengthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxCopyOnAccessArrayLengthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinCopyOnAccessArrayLengthFlag))
{
    Output::Print(u"-%s", u"MinCopyOnAccessArrayLength");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinCopyOnAccessArrayLengthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinCopyOnAccessArrayLengthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinCopyOnAccessArrayLengthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinCopyOnAccessArrayLengthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CopyOnAccessArraySegmentCacheSizeFlag))
{
    Output::Print(u"-%s", u"CopyOnAccessArraySegmentCacheSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CopyOnAccessArraySegmentCacheSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CopyOnAccessArraySegmentCacheSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CopyOnAccessArraySegmentCacheSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CopyOnAccessArraySegmentCacheSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

if (IsEnabled(MinTemplatizedJitRunCountFlag))
{
    Output::Print(u"-%s", u"MinTemplatizedJitRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinTemplatizedJitRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinTemplatizedJitRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinTemplatizedJitRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinTemplatizedJitRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinAsmJsInterpreterRunCountFlag))
{
    Output::Print(u"-%s", u"MinAsmJsInterpreterRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinAsmJsInterpreterRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinAsmJsInterpreterRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinAsmJsInterpreterRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinAsmJsInterpreterRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MinTemplatizedJitLoopRunCountFlag))
{
    Output::Print(u"-%s", u"MinTemplatizedJitLoopRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinTemplatizedJitLoopRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinTemplatizedJitLoopRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinTemplatizedJitLoopRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinTemplatizedJitLoopRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxTemplatizedJitRunCountFlag))
{
    Output::Print(u"-%s", u"MaxTemplatizedJitRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxTemplatizedJitRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxTemplatizedJitRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxTemplatizedJitRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxTemplatizedJitRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MtjrcFlag))
{
    Output::Print(u"-%s", u"Mtjrc");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MtjrcFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MtjrcFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MtjrcFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MtjrcFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxAsmJsInterpreterRunCountFlag))
{
    Output::Print(u"-%s", u"MaxAsmJsInterpreterRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxAsmJsInterpreterRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxAsmJsInterpreterRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxAsmJsInterpreterRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxAsmJsInterpreterRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaicFlag))
{
    Output::Print(u"-%s", u"Maic");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaicFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaicFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaicFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaicFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(AutoProfilingInterpreter0LimitFlag))
{
    Output::Print(u"-%s", u"AutoProfilingInterpreter0Limit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AutoProfilingInterpreter0LimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AutoProfilingInterpreter0LimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AutoProfilingInterpreter0LimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AutoProfilingInterpreter0LimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfilingInterpreter0LimitFlag))
{
    Output::Print(u"-%s", u"ProfilingInterpreter0Limit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfilingInterpreter0LimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfilingInterpreter0LimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfilingInterpreter0LimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfilingInterpreter0LimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AutoProfilingInterpreter1LimitFlag))
{
    Output::Print(u"-%s", u"AutoProfilingInterpreter1Limit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AutoProfilingInterpreter1LimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AutoProfilingInterpreter1LimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AutoProfilingInterpreter1LimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AutoProfilingInterpreter1LimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(SimpleJitLimitFlag))
{
    Output::Print(u"-%s", u"SimpleJitLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SimpleJitLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SimpleJitLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SimpleJitLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SimpleJitLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfilingInterpreter1LimitFlag))
{
    Output::Print(u"-%s", u"ProfilingInterpreter1Limit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfilingInterpreter1LimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfilingInterpreter1LimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfilingInterpreter1LimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfilingInterpreter1LimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ExecutionModeLimitsFlag))
{
    Output::Print(u"-%s", u"ExecutionModeLimits");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExecutionModeLimitsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExecutionModeLimitsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExecutionModeLimitsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExecutionModeLimitsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EmlFlag))
{
    Output::Print(u"-%s", u"Eml");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EmlFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EmlFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EmlFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EmlFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnforceExecutionModeLimitsFlag))
{
    Output::Print(u"-%s", u"EnforceExecutionModeLimits");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnforceExecutionModeLimitsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnforceExecutionModeLimitsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnforceExecutionModeLimitsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnforceExecutionModeLimitsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EemlFlag))
{
    Output::Print(u"-%s", u"Eeml");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EemlFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EemlFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EemlFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EemlFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(SimpleJitAfterFlag))
{
    Output::Print(u"-%s", u"SimpleJitAfter");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SimpleJitAfterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SimpleJitAfterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SimpleJitAfterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SimpleJitAfterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(SjaFlag))
{
    Output::Print(u"-%s", u"Sja");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SjaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SjaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SjaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SjaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FullJitAfterFlag))
{
    Output::Print(u"-%s", u"FullJitAfter");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FullJitAfterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FullJitAfterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FullJitAfterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FullJitAfterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FjaFlag))
{
    Output::Print(u"-%s", u"Fja");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FjaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FjaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FjaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FjaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(NewSimpleJitFlag))
{
    Output::Print(u"-%s", u"NewSimpleJit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NewSimpleJitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NewSimpleJitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NewSimpleJitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NewSimpleJitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MaxLinearIntCaseCountFlag))
{
    Output::Print(u"-%s", u"MaxLinearIntCaseCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxLinearIntCaseCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxLinearIntCaseCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxLinearIntCaseCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxLinearIntCaseCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxSingleCharStrJumpTableSizeFlag))
{
    Output::Print(u"-%s", u"MaxSingleCharStrJumpTableSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxSingleCharStrJumpTableSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxSingleCharStrJumpTableSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxSingleCharStrJumpTableSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxSingleCharStrJumpTableSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxSingleCharStrJumpTableRatioFlag))
{
    Output::Print(u"-%s", u"MaxSingleCharStrJumpTableRatio");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxSingleCharStrJumpTableRatioFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxSingleCharStrJumpTableRatioFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxSingleCharStrJumpTableRatioFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxSingleCharStrJumpTableRatioFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinSwitchJumpTableSizeFlag))
{
    Output::Print(u"-%s", u"MinSwitchJumpTableSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinSwitchJumpTableSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinSwitchJumpTableSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinSwitchJumpTableSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinSwitchJumpTableSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxLinearStringCaseCountFlag))
{
    Output::Print(u"-%s", u"MaxLinearStringCaseCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxLinearStringCaseCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxLinearStringCaseCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxLinearStringCaseCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxLinearStringCaseCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinDeferredFuncTokenCountFlag))
{
    Output::Print(u"-%s", u"MinDeferredFuncTokenCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinDeferredFuncTokenCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinDeferredFuncTokenCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinDeferredFuncTokenCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinDeferredFuncTokenCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if DBG
if (IsEnabled(SkipFuncCountForBailOnNoProfileFlag))
{
    Output::Print(u"-%s", u"SkipFuncCountForBailOnNoProfile");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SkipFuncCountForBailOnNoProfileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SkipFuncCountForBailOnNoProfileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SkipFuncCountForBailOnNoProfileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SkipFuncCountForBailOnNoProfileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(MaxJITFunctionBytecodeByteLengthFlag))
{
    Output::Print(u"-%s", u"MaxJITFunctionBytecodeByteLength");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxJITFunctionBytecodeByteLengthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxJITFunctionBytecodeByteLengthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxJITFunctionBytecodeByteLengthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxJITFunctionBytecodeByteLengthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxJITFunctionBytecodeCountFlag))
{
    Output::Print(u"-%s", u"MaxJITFunctionBytecodeCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxJITFunctionBytecodeCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxJITFunctionBytecodeCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxJITFunctionBytecodeCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxJITFunctionBytecodeCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxLoopsPerFunctionFlag))
{
    Output::Print(u"-%s", u"MaxLoopsPerFunction");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxLoopsPerFunctionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxLoopsPerFunctionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxLoopsPerFunctionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxLoopsPerFunctionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FuncObjectInlineCacheThresholdFlag))
{
    Output::Print(u"-%s", u"FuncObjectInlineCacheThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FuncObjectInlineCacheThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FuncObjectInlineCacheThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FuncObjectInlineCacheThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FuncObjectInlineCacheThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoDeferParseFlag))
{
    Output::Print(u"-%s", u"NoDeferParse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoDeferParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoDeferParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoDeferParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoDeferParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoLogoFlag))
{
    Output::Print(u"-%s", u"NoLogo");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoLogoFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoLogoFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoLogoFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoLogoFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OOPJITMissingOptsFlag))
{
    Output::Print(u"-%s", u"OOPJITMissingOpts");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OOPJITMissingOptsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OOPJITMissingOptsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OOPJITMissingOptsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OOPJITMissingOptsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CrashOnOOPJITFailureFlag))
{
    Output::Print(u"-%s", u"CrashOnOOPJITFailure");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CrashOnOOPJITFailureFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CrashOnOOPJITFailureFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CrashOnOOPJITFailureFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CrashOnOOPJITFailureFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OOPCFGRegistrationFlag))
{
    Output::Print(u"-%s", u"OOPCFGRegistration");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OOPCFGRegistrationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OOPCFGRegistrationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OOPCFGRegistrationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OOPCFGRegistrationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceJITCFGCheckFlag))
{
    Output::Print(u"-%s", u"ForceJITCFGCheck");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceJITCFGCheckFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceJITCFGCheckFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceJITCFGCheckFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceJITCFGCheckFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(UseJITTrampolineFlag))
{
    Output::Print(u"-%s", u"UseJITTrampoline");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(UseJITTrampolineFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(UseJITTrampolineFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(UseJITTrampolineFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(UseJITTrampolineFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoNativeFlag))
{
    Output::Print(u"-%s", u"NoNative");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoNativeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoNativeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoNativeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoNativeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NopFrequencyFlag))
{
    Output::Print(u"-%s", u"NopFrequency");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NopFrequencyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NopFrequencyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NopFrequencyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NopFrequencyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoStrictModeFlag))
{
    Output::Print(u"-%s", u"NoStrictMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoStrictModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoStrictModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoStrictModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoStrictModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NormalizeStatsFlag))
{
    Output::Print(u"-%s", u"NormalizeStats");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NormalizeStatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NormalizeStatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NormalizeStatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NormalizeStatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OffFlag))
{
    Output::Print(u"-%s", u"Off");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OffFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OffFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OffFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OffFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OffProfiledByteCodeFlag))
{
    Output::Print(u"-%s", u"OffProfiledByteCode");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OffProfiledByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OffProfiledByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OffProfiledByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OffProfiledByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OnFlag))
{
    Output::Print(u"-%s", u"On");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OnFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OnFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OnFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OnFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OutputFileFlag))
{
    Output::Print(u"-%s", u"OutputFile");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OutputFileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OutputFileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OutputFileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OutputFileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OutputFileOpenModeFlag))
{
    Output::Print(u"-%s", u"OutputFileOpenMode");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OutputFileOpenModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OutputFileOpenModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OutputFileOpenModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OutputFileOpenModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef ENABLE_TRACE
if (IsEnabled(InMemoryTraceFlag))
{
    Output::Print(u"-%s", u"InMemoryTrace");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InMemoryTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InMemoryTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InMemoryTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InMemoryTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InMemoryTraceBufferSizeFlag))
{
    Output::Print(u"-%s", u"InMemoryTraceBufferSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InMemoryTraceBufferSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InMemoryTraceBufferSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InMemoryTraceBufferSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InMemoryTraceBufferSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef STACK_BACK_TRACE
if (IsEnabled(TraceWithStackFlag))
{
    Output::Print(u"-%s", u"TraceWithStack");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceWithStackFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceWithStackFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceWithStackFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceWithStackFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
if (IsEnabled(PrintRunTimeDataCollectionTraceFlag))
{
    Output::Print(u"-%s", u"PrintRunTimeDataCollectionTrace");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintRunTimeDataCollectionTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintRunTimeDataCollectionTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintRunTimeDataCollectionTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintRunTimeDataCollectionTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef ENABLE_PREJIT
if (IsEnabled(PrejitFlag))
{
    Output::Print(u"-%s", u"Prejit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrejitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrejitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrejitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrejitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(PrintSrcInDumpFlag))
{
    Output::Print(u"-%s", u"PrintSrcInDump");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintSrcInDumpFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintSrcInDumpFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintSrcInDumpFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintSrcInDumpFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if PROFILE_DICTIONARY
if (IsEnabled(ProfileDictionaryFlag))
{
    Output::Print(u"-%s", u"ProfileDictionary");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileDictionaryFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileDictionaryFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileDictionaryFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileDictionaryFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_EXEC
if (IsEnabled(ProfileFlag))
{
    Output::Print(u"-%s", u"Profile");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfileThresholdFlag))
{
    Output::Print(u"-%s", u"ProfileThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_OBJECT_LITERALS
if (IsEnabled(ProfileObjectLiteralFlag))
{
    Output::Print(u"-%s", u"ProfileObjectLiteral");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileObjectLiteralFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileObjectLiteralFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileObjectLiteralFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileObjectLiteralFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_MEM
if (IsEnabled(ProfileMemoryFlag))
{
    Output::Print(u"-%s", u"ProfileMemory");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileMemoryFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileMemoryFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileMemoryFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileMemoryFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_STRINGS
if (IsEnabled(ProfileStringsFlag))
{
    Output::Print(u"-%s", u"ProfileStrings");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileStringsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileStringsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileStringsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileStringsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_TYPES
if (IsEnabled(ProfileTypesFlag))
{
    Output::Print(u"-%s", u"ProfileTypes");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileTypesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileTypesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileTypesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileTypesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_EVALMAP
if (IsEnabled(ProfileEvalMapFlag))
{
    Output::Print(u"-%s", u"ProfileEvalMap");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileEvalMapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileEvalMapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileEvalMapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileEvalMapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
if (IsEnabled(ProfileBailOutRecordMemoryFlag))
{
    Output::Print(u"-%s", u"ProfileBailOutRecordMemory");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileBailOutRecordMemoryFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileBailOutRecordMemoryFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileBailOutRecordMemoryFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileBailOutRecordMemoryFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#if DBG
if (IsEnabled(ValidateIntRangesFlag))
{
    Output::Print(u"-%s", u"ValidateIntRanges");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ValidateIntRangesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ValidateIntRangesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ValidateIntRangesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ValidateIntRangesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(RejitMaxBailOutCountFlag))
{
    Output::Print(u"-%s", u"RejitMaxBailOutCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RejitMaxBailOutCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RejitMaxBailOutCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RejitMaxBailOutCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RejitMaxBailOutCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CallsToBailoutsRatioForRejitFlag))
{
    Output::Print(u"-%s", u"CallsToBailoutsRatioForRejit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CallsToBailoutsRatioForRejitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CallsToBailoutsRatioForRejitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CallsToBailoutsRatioForRejitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CallsToBailoutsRatioForRejitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopIterationsToBailoutsRatioForRejitFlag))
{
    Output::Print(u"-%s", u"LoopIterationsToBailoutsRatioForRejit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopIterationsToBailoutsRatioForRejitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopIterationsToBailoutsRatioForRejitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopIterationsToBailoutsRatioForRejitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopIterationsToBailoutsRatioForRejitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinBailOutsBeforeRejitFlag))
{
    Output::Print(u"-%s", u"MinBailOutsBeforeRejit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinBailOutsBeforeRejitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinBailOutsBeforeRejitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinBailOutsBeforeRejitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinBailOutsBeforeRejitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinBailOutsBeforeRejitForLoopsFlag))
{
    Output::Print(u"-%s", u"MinBailOutsBeforeRejitForLoops");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinBailOutsBeforeRejitForLoopsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinBailOutsBeforeRejitForLoopsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinBailOutsBeforeRejitForLoopsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinBailOutsBeforeRejitForLoopsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LibraryStackFrameFlag))
{
    Output::Print(u"-%s", u"LibraryStackFrame");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LibraryStackFrameFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LibraryStackFrameFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LibraryStackFrameFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LibraryStackFrameFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LibraryStackFrameDebuggerFlag))
{
    Output::Print(u"-%s", u"LibraryStackFrameDebugger");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LibraryStackFrameDebuggerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LibraryStackFrameDebuggerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LibraryStackFrameDebuggerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LibraryStackFrameDebuggerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef RECYCLER_STRESS
if (IsEnabled(RecyclerStressFlag))
{
    Output::Print(u"-%s", u"RecyclerStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if ENABLE_CONCURRENT_GC
if (IsEnabled(RecyclerBackgroundStressFlag))
{
    Output::Print(u"-%s", u"RecyclerBackgroundStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerBackgroundStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerBackgroundStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerBackgroundStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerBackgroundStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerConcurrentStressFlag))
{
    Output::Print(u"-%s", u"RecyclerConcurrentStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerConcurrentStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerConcurrentStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerConcurrentStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerConcurrentStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerConcurrentRepeatStressFlag))
{
    Output::Print(u"-%s", u"RecyclerConcurrentRepeatStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerConcurrentRepeatStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerConcurrentRepeatStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerConcurrentRepeatStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerConcurrentRepeatStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#if ENABLE_PARTIAL_GC
if (IsEnabled(RecyclerPartialStressFlag))
{
    Output::Print(u"-%s", u"RecyclerPartialStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerPartialStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerPartialStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerPartialStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerPartialStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(RecyclerTrackStressFlag))
{
    Output::Print(u"-%s", u"RecyclerTrackStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerTrackStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerTrackStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerTrackStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerTrackStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerInduceFalsePositivesFlag))
{
    Output::Print(u"-%s", u"RecyclerInduceFalsePositives");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerInduceFalsePositivesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerInduceFalsePositivesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerInduceFalsePositivesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerInduceFalsePositivesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif // RECYCLER_STRESS
if (IsEnabled(RecyclerForceMarkInteriorFlag))
{
    Output::Print(u"-%s", u"RecyclerForceMarkInterior");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerForceMarkInteriorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerForceMarkInteriorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerForceMarkInteriorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerForceMarkInteriorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if ENABLE_CONCURRENT_GC
if (IsEnabled(RecyclerPriorityBoostTimeoutFlag))
{
    Output::Print(u"-%s", u"RecyclerPriorityBoostTimeout");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerPriorityBoostTimeoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerPriorityBoostTimeoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerPriorityBoostTimeoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerPriorityBoostTimeoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerThreadCollectTimeoutFlag))
{
    Output::Print(u"-%s", u"RecyclerThreadCollectTimeout");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerThreadCollectTimeoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerThreadCollectTimeoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerThreadCollectTimeoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerThreadCollectTimeoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableConcurrentSweepAllocFlag))
{
    Output::Print(u"-%s", u"EnableConcurrentSweepAlloc");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableConcurrentSweepAllocFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableConcurrentSweepAllocFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableConcurrentSweepAllocFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableConcurrentSweepAllocFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ecsaFlag))
{
    Output::Print(u"-%s", u"ecsa");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ecsaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ecsaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ecsaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ecsaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef RECYCLER_PAGE_HEAP
if (IsEnabled(PageHeapFlag))
{
    Output::Print(u"-%s", u"PageHeap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapAllocStackFlag))
{
    Output::Print(u"-%s", u"PageHeapAllocStack");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapAllocStackFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapAllocStackFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapAllocStackFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapAllocStackFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapFreeStackFlag))
{
    Output::Print(u"-%s", u"PageHeapFreeStack");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapFreeStackFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapFreeStackFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapFreeStackFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapFreeStackFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapBucketNumberFlag))
{
    Output::Print(u"-%s", u"PageHeapBucketNumber");
    switch (FlagNumberRange)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapBucketNumberFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapBucketNumberFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapBucketNumberFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapBucketNumberFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapBlockTypeFlag))
{
    Output::Print(u"-%s", u"PageHeapBlockType");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapBlockTypeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapBlockTypeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapBlockTypeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapBlockTypeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapDecommitGuardPageFlag))
{
    Output::Print(u"-%s", u"PageHeapDecommitGuardPage");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapDecommitGuardPageFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapDecommitGuardPageFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapDecommitGuardPageFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapDecommitGuardPageFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef RECYCLER_NO_PAGE_REUSE
if (IsEnabled(RecyclerNoPageReuseFlag))
{
    Output::Print(u"-%s", u"RecyclerNoPageReuse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerNoPageReuseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerNoPageReuseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerNoPageReuseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerNoPageReuseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef RECYCLER_MEMORY_VERIFY
if (IsEnabled(RecyclerVerifyFlag))
{
    Output::Print(u"-%s", u"RecyclerVerify");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerVerifyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerVerifyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerVerifyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerVerifyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerVerifyPadSizeFlag))
{
    Output::Print(u"-%s", u"RecyclerVerifyPadSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerVerifyPadSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerVerifyPadSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerVerifyPadSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerVerifyPadSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(RecyclerTestFlag))
{
    Output::Print(u"-%s", u"RecyclerTest");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerTestFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerTestFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerTestFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerTestFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerProtectPagesOnRescanFlag))
{
    Output::Print(u"-%s", u"RecyclerProtectPagesOnRescan");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerProtectPagesOnRescanFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerProtectPagesOnRescanFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerProtectPagesOnRescanFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerProtectPagesOnRescanFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef RECYCLER_VERIFY_MARK
if (IsEnabled(RecyclerVerifyMarkFlag))
{
    Output::Print(u"-%s", u"RecyclerVerifyMark");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerVerifyMarkFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerVerifyMarkFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerVerifyMarkFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerVerifyMarkFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(LowMemoryCapFlag))
{
    Output::Print(u"-%s", u"LowMemoryCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LowMemoryCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LowMemoryCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LowMemoryCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LowMemoryCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NewPagesCapDuringBGSweepingFlag))
{
    Output::Print(u"-%s", u"NewPagesCapDuringBGSweeping");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NewPagesCapDuringBGSweepingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NewPagesCapDuringBGSweepingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NewPagesCapDuringBGSweepingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NewPagesCapDuringBGSweepingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AllocPolicyLimitFlag))
{
    Output::Print(u"-%s", u"AllocPolicyLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AllocPolicyLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AllocPolicyLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AllocPolicyLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AllocPolicyLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef RUNTIME_DATA_COLLECTION
if (IsEnabled(RuntimeDataOutputFileFlag))
{
    Output::Print(u"-%s", u"RuntimeDataOutputFile");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RuntimeDataOutputFileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RuntimeDataOutputFileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RuntimeDataOutputFileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RuntimeDataOutputFileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(SpeculationCapFlag))
{
    Output::Print(u"-%s", u"SpeculationCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SpeculationCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SpeculationCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SpeculationCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SpeculationCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
if (IsEnabled(StatsFlag))
{
    Output::Print(u"-%s", u"Stats");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#if EXCEPTION_RECOVERY
if (IsEnabled(SwallowExceptionsFlag))
{
    Output::Print(u"-%s", u"SwallowExceptions");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SwallowExceptionsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SwallowExceptionsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SwallowExceptionsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SwallowExceptionsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(PrintSystemExceptionFlag))
{
    Output::Print(u"-%s", u"PrintSystemException");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintSystemExceptionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintSystemExceptionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintSystemExceptionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintSystemExceptionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(SwitchOptHolesThresholdFlag))
{
    Output::Print(u"-%s", u"SwitchOptHolesThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SwitchOptHolesThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SwitchOptHolesThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SwitchOptHolesThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SwitchOptHolesThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TempMinFlag))
{
    Output::Print(u"-%s", u"TempMin");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TempMinFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TempMinFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TempMinFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TempMinFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TempMaxFlag))
{
    Output::Print(u"-%s", u"TempMax");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TempMaxFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TempMaxFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TempMaxFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TempMaxFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceFlag))
{
    Output::Print(u"-%s", u"Trace");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if defined(_M_X64)
if (IsEnabled(LoopAlignNopLimitFlag))
{
    Output::Print(u"-%s", u"LoopAlignNopLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopAlignNopLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopAlignNopLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopAlignNopLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopAlignNopLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#ifdef PROFILE_MEM
if (IsEnabled(TraceMemoryFlag))
{
    Output::Print(u"-%s", u"TraceMemory");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceMemoryFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceMemoryFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceMemoryFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceMemoryFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#if DBG_DUMP || defined(RECYCLER_TRACE)
//TraceMetaDataParsing flag with optional levels:
//    Level 1 = interfaces only
//    Level 2 = interfaces and methods
//    Level 3 = interfaces, methods and parameters
//    Level 4 = interfaces and properties
//    Level 5 (default) = ALL
if (IsEnabled(TraceMetaDataParsingFlag))
{
    Output::Print(u"-%s", u"TraceMetaDataParsing");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceMetaDataParsingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceMetaDataParsingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceMetaDataParsingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceMetaDataParsingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceWin8AllocationsFlag))
{
    Output::Print(u"-%s", u"TraceWin8Allocations");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceWin8AllocationsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceWin8AllocationsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceWin8AllocationsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceWin8AllocationsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceWin8DeallocationsImmediateFlag))
{
    Output::Print(u"-%s", u"TraceWin8DeallocationsImmediate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceWin8DeallocationsImmediateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceWin8DeallocationsImmediateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceWin8DeallocationsImmediateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceWin8DeallocationsImmediateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PrintWin8StatsDetailedFlag))
{
    Output::Print(u"-%s", u"PrintWin8StatsDetailed");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintWin8StatsDetailedFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintWin8StatsDetailedFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintWin8StatsDetailedFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintWin8StatsDetailedFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceProtectPagesFlag))
{
    Output::Print(u"-%s", u"TraceProtectPages");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceProtectPagesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceProtectPagesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceProtectPagesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceProtectPagesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(TraceAsyncDebugCallsFlag))
{
    Output::Print(u"-%s", u"TraceAsyncDebugCalls");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceAsyncDebugCallsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceAsyncDebugCallsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceAsyncDebugCallsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceAsyncDebugCallsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef TRACK_DISPATCH
if (IsEnabled(TrackDispatchFlag))
{
    Output::Print(u"-%s", u"TrackDispatch");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TrackDispatchFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TrackDispatchFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TrackDispatchFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TrackDispatchFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(VerboseFlag))
{
    Output::Print(u"-%s", u"Verbose");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(VerboseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(VerboseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(VerboseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(VerboseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(UseFullNameFlag))
{
    Output::Print(u"-%s", u"UseFullName");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(UseFullNameFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(UseFullNameFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(UseFullNameFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(UseFullNameFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(Utf8Flag))
{
    Output::Print(u"-%s", u"Utf8");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(Utf8Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(Utf8Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(Utf8Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(Utf8Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(VersionFlag))
{
    Output::Print(u"-%s", u"Version");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(VersionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(VersionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(VersionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(VersionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WERExceptionSupportFlag))
{
    Output::Print(u"-%s", u"WERExceptionSupport");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WERExceptionSupportFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WERExceptionSupportFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WERExceptionSupportFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WERExceptionSupportFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ExtendedErrorStackForTestHostFlag))
{
    Output::Print(u"-%s", u"ExtendedErrorStackForTestHost");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExtendedErrorStackForTestHostFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExtendedErrorStackForTestHostFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExtendedErrorStackForTestHostFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExtendedErrorStackForTestHostFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(errorStackTraceFlag))
{
    Output::Print(u"-%s", u"errorStackTrace");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(errorStackTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(errorStackTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(errorStackTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(errorStackTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DoHeapEnumOnEngineShutdownFlag))
{
    Output::Print(u"-%s", u"DoHeapEnumOnEngineShutdown");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DoHeapEnumOnEngineShutdownFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DoHeapEnumOnEngineShutdownFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DoHeapEnumOnEngineShutdownFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DoHeapEnumOnEngineShutdownFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef HEAP_ENUMERATION_VALIDATION
if (IsEnabled(ValidateHeapEnumFlag))
{
    Output::Print(u"-%s", u"ValidateHeapEnum");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ValidateHeapEnumFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ValidateHeapEnumFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ValidateHeapEnumFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ValidateHeapEnumFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
if (IsEnabled(RegexTracingFlag))
{
    Output::Print(u"-%s", u"RegexTracing");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexTracingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexTracingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexTracingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexTracingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexProfileFlag))
{
    Output::Print(u"-%s", u"RegexProfile");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexProfileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexProfileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexProfileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexProfileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexDebugFlag))
{
    Output::Print(u"-%s", u"RegexDebug");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexDebugFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexDebugFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexDebugFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexDebugFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexDebugASTFlag))
{
    Output::Print(u"-%s", u"RegexDebugAST");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexDebugASTFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexDebugASTFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexDebugASTFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexDebugASTFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexDebugAnnotatedASTFlag))
{
    Output::Print(u"-%s", u"RegexDebugAnnotatedAST");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexDebugAnnotatedASTFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexDebugAnnotatedASTFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexDebugAnnotatedASTFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexDebugAnnotatedASTFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexBytecodeDebugFlag))
{
    Output::Print(u"-%s", u"RegexBytecodeDebug");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexBytecodeDebugFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexBytecodeDebugFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexBytecodeDebugFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexBytecodeDebugFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexOptimizeFlag))
{
    Output::Print(u"-%s", u"RegexOptimize");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexOptimizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexOptimizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexOptimizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexOptimizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DynamicRegexMruListSizeFlag))
{
    Output::Print(u"-%s", u"DynamicRegexMruListSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DynamicRegexMruListSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DynamicRegexMruListSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DynamicRegexMruListSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DynamicRegexMruListSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

if (IsEnabled(OptimizeForManyInstancesFlag))
{
    Output::Print(u"-%s", u"OptimizeForManyInstances");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OptimizeForManyInstancesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OptimizeForManyInstancesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OptimizeForManyInstancesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OptimizeForManyInstancesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableArrayTypeMutationFlag))
{
    Output::Print(u"-%s", u"EnableArrayTypeMutation");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableArrayTypeMutationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableArrayTypeMutationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableArrayTypeMutationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableArrayTypeMutationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ArrayMutationTestSeedFlag))
{
    Output::Print(u"-%s", u"ArrayMutationTestSeed");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArrayMutationTestSeedFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArrayMutationTestSeedFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArrayMutationTestSeedFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArrayMutationTestSeedFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TestTraceFlag))
{
    Output::Print(u"-%s", u"TestTrace");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TestTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TestTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TestTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TestTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableEvalMapCleanupFlag))
{
    Output::Print(u"-%s", u"EnableEvalMapCleanup");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableEvalMapCleanupFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableEvalMapCleanupFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableEvalMapCleanupFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableEvalMapCleanupFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef PROFILE_MEM
if (IsEnabled(TraceObjectAllocationFlag))
{
    Output::Print(u"-%s", u"TraceObjectAllocation");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceObjectAllocationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceObjectAllocationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceObjectAllocationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceObjectAllocationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(SseFlag))
{
    Output::Print(u"-%s", u"Sse");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DeletedPropertyReuseThresholdFlag))
{
    Output::Print(u"-%s", u"DeletedPropertyReuseThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DeletedPropertyReuseThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DeletedPropertyReuseThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DeletedPropertyReuseThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DeletedPropertyReuseThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceStringKeyedSimpleDictionaryTypeHandlerFlag))
{
    Output::Print(u"-%s", u"ForceStringKeyedSimpleDictionaryTypeHandler");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceStringKeyedSimpleDictionaryTypeHandlerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceStringKeyedSimpleDictionaryTypeHandlerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceStringKeyedSimpleDictionaryTypeHandlerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceStringKeyedSimpleDictionaryTypeHandlerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BigDictionaryTypeHandlerThresholdFlag))
{
    Output::Print(u"-%s", u"BigDictionaryTypeHandlerThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BigDictionaryTypeHandlerThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BigDictionaryTypeHandlerThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BigDictionaryTypeHandlerThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BigDictionaryTypeHandlerThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TypeSnapshotEnumerationFlag))
{
    Output::Print(u"-%s", u"TypeSnapshotEnumeration");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TypeSnapshotEnumerationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TypeSnapshotEnumerationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TypeSnapshotEnumerationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TypeSnapshotEnumerationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(IsolatePrototypesFlag))
{
    Output::Print(u"-%s", u"IsolatePrototypes");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IsolatePrototypesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IsolatePrototypesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IsolatePrototypesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IsolatePrototypesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ChangeTypeOnProtoFlag))
{
    Output::Print(u"-%s", u"ChangeTypeOnProto");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ChangeTypeOnProtoFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ChangeTypeOnProtoFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ChangeTypeOnProtoFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ChangeTypeOnProtoFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ShareInlineCachesFlag))
{
    Output::Print(u"-%s", u"ShareInlineCaches");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ShareInlineCachesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ShareInlineCachesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ShareInlineCachesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ShareInlineCachesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisableDebugObjectFlag))
{
    Output::Print(u"-%s", u"DisableDebugObject");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisableDebugObjectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisableDebugObjectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisableDebugObjectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisableDebugObjectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpHeapFlag))
{
    Output::Print(u"-%s", u"DumpHeap");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpHeapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpHeapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpHeapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpHeapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(autoProxyFlag))
{
    Output::Print(u"-%s", u"autoProxy");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(autoProxyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(autoProxyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(autoProxyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(autoProxyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PerfHintLevelFlag))
{
    Output::Print(u"-%s", u"PerfHintLevel");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PerfHintLevelFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PerfHintLevelFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PerfHintLevelFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PerfHintLevelFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
if (IsEnabled(MemProtectHeapFlag))
{
    Output::Print(u"-%s", u"MemProtectHeap");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef RECYCLER_STRESS
if (IsEnabled(MemProtectHeapStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if ENABLE_CONCURRENT_GC
if (IsEnabled(MemProtectHeapBackgroundStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapBackgroundStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapBackgroundStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapBackgroundStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapBackgroundStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapBackgroundStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MemProtectHeapConcurrentStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapConcurrentStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapConcurrentStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapConcurrentStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapConcurrentStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapConcurrentStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MemProtectHeapConcurrentRepeatStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapConcurrentRepeatStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapConcurrentRepeatStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapConcurrentRepeatStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapConcurrentRepeatStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapConcurrentRepeatStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#if ENABLE_PARTIAL_GC
if (IsEnabled(MemProtectHeapPartialStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapPartialStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapPartialStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapPartialStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapPartialStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapPartialStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
if (IsEnabled(FixPropsOnPathTypesFlag))
{
    Output::Print(u"-%s", u"FixPropsOnPathTypes");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FixPropsOnPathTypesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FixPropsOnPathTypesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FixPropsOnPathTypesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FixPropsOnPathTypesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(BailoutTraceFilterFlag))
{
    Output::Print(u"-%s", u"BailoutTraceFilter");
    switch (FlagNumberSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailoutTraceFilterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailoutTraceFilterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailoutTraceFilterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailoutTraceFilterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RejitTraceFilterFlag))
{
    Output::Print(u"-%s", u"RejitTraceFilter");
    switch (FlagNumberSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RejitTraceFilterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RejitTraceFilterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RejitTraceFilterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RejitTraceFilterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// recycler heuristic flags
if (IsEnabled(MaxBackgroundFinishMarkCountFlag))
{
    Output::Print(u"-%s", u"MaxBackgroundFinishMarkCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxBackgroundFinishMarkCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxBackgroundFinishMarkCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxBackgroundFinishMarkCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxBackgroundFinishMarkCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BackgroundFinishMarkWaitTimeFlag))
{
    Output::Print(u"-%s", u"BackgroundFinishMarkWaitTime");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BackgroundFinishMarkWaitTimeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BackgroundFinishMarkWaitTimeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BackgroundFinishMarkWaitTimeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BackgroundFinishMarkWaitTimeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinBackgroundRepeatMarkRescanBytesFlag))
{
    Output::Print(u"-%s", u"MinBackgroundRepeatMarkRescanBytes");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinBackgroundRepeatMarkRescanBytesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinBackgroundRepeatMarkRescanBytesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinBackgroundRepeatMarkRescanBytesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinBackgroundRepeatMarkRescanBytesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if defined(_M_X64)
if (IsEnabled(ZeroMemoryWithNonTemporalStoreFlag))
{
    Output::Print(u"-%s", u"ZeroMemoryWithNonTemporalStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ZeroMemoryWithNonTemporalStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ZeroMemoryWithNonTemporalStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ZeroMemoryWithNonTemporalStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ZeroMemoryWithNonTemporalStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

// recycler memory restrict test flags
if (IsEnabled(MaxMarkStackPageCountFlag))
{
    Output::Print(u"-%s", u"MaxMarkStackPageCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxMarkStackPageCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxMarkStackPageCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxMarkStackPageCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxMarkStackPageCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxTrackedObjectListCountFlag))
{
    Output::Print(u"-%s", u"MaxTrackedObjectListCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxTrackedObjectListCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxTrackedObjectListCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxTrackedObjectListCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxTrackedObjectListCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// make the recycler page integration path easier to hit
if (IsEnabled(NumberAllocPlusSizeFlag))
{
    Output::Print(u"-%s", u"NumberAllocPlusSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NumberAllocPlusSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NumberAllocPlusSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NumberAllocPlusSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NumberAllocPlusSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if DBG
if (IsEnabled(InitializeInterpreterSlotsWithInvalidStackVarFlag))
{
    Output::Print(u"-%s", u"InitializeInterpreterSlotsWithInvalidStackVar");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InitializeInterpreterSlotsWithInvalidStackVarFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InitializeInterpreterSlotsWithInvalidStackVarFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InitializeInterpreterSlotsWithInvalidStackVarFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InitializeInterpreterSlotsWithInvalidStackVarFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#if DBG
if (IsEnabled(PRNGSeed0Flag))
{
    Output::Print(u"-%s", u"PRNGSeed0");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PRNGSeed0Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PRNGSeed0Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PRNGSeed0Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PRNGSeed0Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PRNGSeed1Flag))
{
    Output::Print(u"-%s", u"PRNGSeed1");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PRNGSeed1Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PRNGSeed1Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PRNGSeed1Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PRNGSeed1Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

if (IsEnabled(ClearInlineCachesOnCollectFlag))
{
    Output::Print(u"-%s", u"ClearInlineCachesOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ClearInlineCachesOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ClearInlineCachesOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ClearInlineCachesOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ClearInlineCachesOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineCacheInvalidationListCompactionThresholdFlag))
{
    Output::Print(u"-%s", u"InlineCacheInvalidationListCompactionThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineCacheInvalidationListCompactionThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineCacheInvalidationListCompactionThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineCacheInvalidationListCompactionThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineCacheInvalidationListCompactionThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConstructorCacheInvalidationThresholdFlag))
{
    Output::Print(u"-%s", u"ConstructorCacheInvalidationThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConstructorCacheInvalidationThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConstructorCacheInvalidationThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConstructorCacheInvalidationThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConstructorCacheInvalidationThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifdef IR_VIEWER
if (IsEnabled(IRViewerFlag))
{
    Output::Print(u"-%s", u"IRViewer");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IRViewerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IRViewerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IRViewerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IRViewerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif /* IR_VIEWER */

if (IsEnabled(GCMemoryThresholdFlag))
{
    Output::Print(u"-%s", u"GCMemoryThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(GCMemoryThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(GCMemoryThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(GCMemoryThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(GCMemoryThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if DBG
    if (IsEnabled(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag))
{
    Output::Print(u"-%s", u"SimulatePolyCacheWithOneTypeForInlineCacheIndex");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

if (IsEnabled(JITServerIdleTimeoutFlag))
{
    Output::Print(u"-%s", u"JITServerIdleTimeout");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JITServerIdleTimeoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JITServerIdleTimeoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JITServerIdleTimeoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JITServerIdleTimeoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(JITServerMaxInactivePageAllocatorCountFlag))
{
    Output::Print(u"-%s", u"JITServerMaxInactivePageAllocatorCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JITServerMaxInactivePageAllocatorCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JITServerMaxInactivePageAllocatorCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JITServerMaxInactivePageAllocatorCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JITServerMaxInactivePageAllocatorCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(StrictWriteBarrierCheckFlag))
{
    Output::Print(u"-%s", u"StrictWriteBarrierCheck");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StrictWriteBarrierCheckFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StrictWriteBarrierCheckFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StrictWriteBarrierCheckFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StrictWriteBarrierCheckFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WriteBarrierTestFlag))
{
    Output::Print(u"-%s", u"WriteBarrierTest");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WriteBarrierTestFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WriteBarrierTestFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WriteBarrierTestFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WriteBarrierTestFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSoftwareWriteBarrierFlag))
{
    Output::Print(u"-%s", u"ForceSoftwareWriteBarrier");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSoftwareWriteBarrierFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSoftwareWriteBarrierFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSoftwareWriteBarrierFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSoftwareWriteBarrierFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(VerifyBarrierBitFlag))
{
    Output::Print(u"-%s", u"VerifyBarrierBit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(VerifyBarrierBitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(VerifyBarrierBitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(VerifyBarrierBitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(VerifyBarrierBitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableBGFreeZeroFlag))
{
    Output::Print(u"-%s", u"EnableBGFreeZero");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableBGFreeZeroFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableBGFreeZeroFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableBGFreeZeroFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableBGFreeZeroFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(KeepRecyclerTrackDataFlag))
{
    Output::Print(u"-%s", u"KeepRecyclerTrackData");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(KeepRecyclerTrackDataFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(KeepRecyclerTrackDataFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(KeepRecyclerTrackDataFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(KeepRecyclerTrackDataFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MaxSingleAllocSizeInMBFlag))
{
    Output::Print(u"-%s", u"MaxSingleAllocSizeInMB");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxSingleAllocSizeInMBFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxSingleAllocSizeInMBFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxSingleAllocSizeInMBFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxSingleAllocSizeInMBFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetDefaultValueAsBoolean
    ///
    /// Get the default value of a given boolean flag. If the flag is not of boolean
    /// type, will assert on CHK or return FALSE on FRE.
    ///
    ///----------------------------------------------------------------------------
    Boolean
    ConfigFlagsTable::GetDefaultValueAsBoolean(Flag flag) const
    {
        Boolean retValue = FALSE;

        switch (flag)
        {
        #if DBG
        case ArrayValidateFlag:
            retValue = (Boolean) false;
            break;
        case MemOpMissingValueValidateFlag:
            retValue = (Boolean) false;
            break;
        case OOPJITFixupValidateFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #ifdef ARENA_MEMORY_VERIFY
        case ArenaNoFreeListFlag:
            retValue = (Boolean) false;
            break;
        case ArenaNoPageReuseFlag:
            retValue = (Boolean) false;
            break;
        case ArenaUseHeapAllocFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case ValidateInlineStackFlag:
            retValue = (Boolean) false;
            break;
        case AsmDiffFlag:
            retValue = (Boolean) false;
            break;
        case AsmJsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AsmJs;
            break;
        case AsmJsStopOnErrorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AsmJsStopOnError;
            break;
        case AsmJsEdgeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AsmJsEdge;
            break;
        case WasmFlag:
            retValue = (Boolean) DEFAULT_CONFIG_Wasm;
            break;
        case WasmI64Flag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmI64;
            break;
        case WasmFastArrayFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmFastArray;
            break;
        case WasmSharedArrayVirtualBufferFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmSharedArrayVirtualBuffer;
            break;
        case WasmMathExFilterFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmMathExFilter;
            break;
        case WasmCheckVersionFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmCheckVersion;
            break;
        case WasmAssignModuleIDFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmAssignModuleID;
            break;
        case WasmIgnoreLimitsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmIgnoreLimits;
            break;
        case WasmFoldFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmFold;
            break;
        case WasmIgnoreResponseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmIgnoreResponse;
            break;
        case WasmThreadsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmThreads;
            break;
        case WasmMultiValueFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmMultiValue;
            break;
        case WasmSignExtendsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmSignExtends;
            break;
        case WasmNontrappingFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmNontrapping;
            break;

        // WebAssembly Experimental Features
        // Master WasmExperimental flag to activate WebAssembly experimental features
        case WasmExperimentalFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmExperimental;
            break;

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        case WasmSimdFlag:
            retValue = (Boolean) true;
            break;

        case AssertBreakFlag:
            retValue = (Boolean) false;
            break;
        case AssertPopUpFlag:
            retValue = (Boolean) false;
            break;
        case AssertIgnoreFlag:
            retValue = (Boolean) false;
            break;
        case AsyncDebuggingFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AsyncDebugging;
            break;
        case BaselineModeFlag:
            retValue = (Boolean) false;
            break;
        #ifdef BAILOUT_INJECTION
        case BailOutAtEveryLineFlag:
            retValue = (Boolean) false;
            break;
        case BailOutAtEveryByteCodeFlag:
            retValue = (Boolean) false;
            break;
        case BailOutAtEveryImplicitCallFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case BenchmarkFlag:
            retValue = (Boolean) false;
            break;
        case BgJitFlag:
            retValue = (Boolean) true;
            break;
        case BgParseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_BgParse;
            break;

        case CreateFunctionProxyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CreateFunctionProxy;
            break;
        case HybridFgJitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_HybridFgJit;
            break;
        case BytecodeHistFlag:
            retValue = (Boolean) false;
            break;
        case CurrentSourceInfoFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CurrentSourceInfo;
            break;
        case CFGLogFlag:
            retValue = (Boolean) false;
            break;
        case CheckAlignmentFlag:
            retValue = (Boolean) false;
            break;
        case CheckEmitBufferPermissionsFlag:
            retValue = (Boolean) false;
            break;
        #ifdef CHECK_MEMORY_LEAK
        case CheckMemoryLeakFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case CheckOpHelpersFlag:
            retValue = (Boolean) false;
            break;
        case CloneInlinedPolymorphicCachesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CloneInlinedPolymorphicCaches;
            break;
        case ConcurrentRuntimeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ConcurrentRuntime;
            break;
        case DebugFlag:
            retValue = (Boolean) false;
            break;

        case DebugWindowFlag:
            retValue = (Boolean) false;
            break;
        case ParserStateCacheFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ParserStateCache;
            break;
        case CompressParserStateCacheFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CompressParserStateCache;
            break;
        case DeferTopLevelTillFirstCallFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DeferTopLevelTillFirstCall;
            break;
        case DirectCallTelemetryStatsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DirectCallTelemetryStats;
            break;
        case DisableArrayBTreeFlag:
            retValue = (Boolean) false;
            break;
        case DisableRentalThreadingFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DisableRentalThreading;
            break;
        case DisableVTuneSourceLineInfoFlag:
            retValue = (Boolean) false;
            break;
        case DisplayMemStatsFlag:
            retValue = (Boolean) false;
            break;
        #ifdef DUMP_FRAGMENTATION_STATS
        case DumpFragmentationStatsFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case DumpIRAddressesFlag:
            retValue = (Boolean) false;
            break;
        case DumpLineNoInColorFlag:
            retValue = (Boolean) false;
            break;
        #ifdef RECYCLER_DUMP_OBJECT_GRAPH
        case DumpObjectGraphOnExitFlag:
            retValue = (Boolean) false;
            break;
        case DumpObjectGraphOnCollectFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case DumpEvalStringOnRemovalFlag:
            retValue = (Boolean) false;
            break;
        case DumpObjectGraphOnEnumFlag:
            retValue = (Boolean) false;
            break;
        #ifdef EDIT_AND_CONTINUE
        case EditTestFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case WininetProfileCacheFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WininetProfileCache;
            break;
        case NoDynamicProfileInMemoryCacheFlag:
            retValue = (Boolean) false;
            break;
        case ProfileBasedSpeculativeJitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ProfileBasedSpeculativeJit;
            break;
        case ExecuteByteCodeBufferReturnsInvalidByteCodeFlag:
            retValue = (Boolean) false;
            break;
        case SkipSplitOnNoResultFlag:
            retValue = (Boolean) DEFAULT_CONFIG_SkipSplitWhenResultIgnored;
            break;
        case Force32BitByteCodeFlag:
            retValue = (Boolean) false;
            break;

        case CollectGarbageFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CollectGarbage;
            break;

        case IntlFlag:
            retValue = (Boolean) DEFAULT_CONFIG_Intl;
            break;
        case IntlBuiltInsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_IntlBuiltIns;
            break;
        case IntlPlatformFlag:
            retValue = (Boolean) DEFAULT_CONFIG_IntlPlatform;
            break;

        case JsBuiltInFlag:
            retValue = (Boolean) DEFAULT_CONFIG_JsBuiltIn;
            break;
        case JitReproFlag:
            retValue = (Boolean) DEFAULT_CONFIG_JitRepro;
            break;
        case EntryPointInfoRpcDataFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EntryPointInfoRpcData;
            break;

        case LdChakraLibFlag:
            retValue = (Boolean) DEFAULT_CONFIG_LdChakraLib;
            break;
        case TestChakraLibFlag:
            retValue = (Boolean) DEFAULT_CONFIG_TestChakraLib;
            break;

        // ES6 (BLUE+1) features/flags

        // Master ES6 flag to enable STABLE ES6 features/flags
        case ES6Flag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6;
            break;

        // Master ES6 flag to enable ALL sub ES6 features/flags
        case ES6AllFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6All;
            break;

        // Master ES6 flag to enable Threshold ES6 features/flags
        case ES6ExperimentalFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6All;
            break;

        // Per ES6 feature/flag

        case ES7AsyncAwaitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES7AsyncAwait;
            break;
        case ES6DateParseFixFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6DateParseFix;
            break;
        case ES6FunctionNameFullFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6FunctionNameFull;
            break;
        case ES6GeneratorsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Generators;
            break;
        case ES7ExponentiationOperatorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES7ExponentionOperator;
            break;

        case ES7ValuesEntriesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES7ValuesEntries;
            break;
        case ES7TrailingCommaFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES7TrailingComma;
            break;
        case ES6IsConcatSpreadableFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6IsConcatSpreadable;
            break;
        case ES6MathFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Math;
            break;

        #ifndef COMPILE_DISABLE_ESDynamicImport
        #define COMPILE_DISABLE_ESDynamicImport 0
        #endif
        case ESDynamicImportFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESDynamicImport;
            break;

        case ES6ModuleFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Module;
            break;
        case ES6ObjectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Object;
            break;
        case ES6NumberFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Number;
            break;
        case ES6ObjectLiteralsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6ObjectLiterals;
            break;
        case ES6ProxyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Proxy;
            break;
        case ES6RestFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Rest;
            break;
        case ES6SpreadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Spread;
            break;
        case ES6StringFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6String;
            break;
        case ES6StringPrototypeFixesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6StringPrototypeFixes;
            break;
        case ES2018ObjectRestSpreadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES2018ObjectRestSpread;
            break;

        case ES6PrototypeChainFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6PrototypeChain;
            break;
        case ES6ToPrimitiveFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6ToPrimitive;
            break;
        case ES6ToLengthFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6ToLength;
            break;
        case ES6ToStringTagFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6ToStringTag;
            break;
        case ES6UnicodeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Unicode;
            break;
        case ES6UnicodeVerboseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6UnicodeVerbose;
            break;
        case ES6UnscopablesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Unscopables;
            break;
        case ES6RegExStickyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6RegExSticky;
            break;
        case ES2018RegExDotAllFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES2018RegExDotAll;
            break;
        case ESExportNsAsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESExportNsAs;
            break;
        case ES2018AsyncIterationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES2018AsyncIteration;
            break;
        case ESTopLevelAwaitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESTopLevelAwait;
            break;

        #ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
            #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
        #endif
        case ES6RegExPrototypePropertiesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6RegExPrototypeProperties;
            break;

        #ifndef COMPILE_DISABLE_ES6RegExSymbols
            #define COMPILE_DISABLE_ES6RegExSymbols 0
        #endif

        // When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
        // sets implicit call flag before calling into script
        // Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        case ES6RegExSymbolsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6RegExSymbols;
            break;

        case ES6VerboseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Verbose;
            break;
        case ESObjectGetOwnPropertyDescriptorsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESObjectGetOwnPropertyDescriptors;
            break;

        #ifndef COMPILE_DISABLE_ESSharedArrayBuffer
            #define COMPILE_DISABLE_ESSharedArrayBuffer 0
        #endif

        case ESSharedArrayBufferFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESSharedArrayBuffer;
            break;

        // ES BigInt flag
        case ESBigIntFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESBigInt;
            break;

        // ES Numeric Separator support for numeric constants
        case ESNumericSeparatorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESNumericSeparator;
            break;

        // ES Nullish coalescing operator support (??)
        case ESNullishCoalescingOperatorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESNullishCoalescingOperator;
            break;

        // ES Hashbang support for interpreter directive syntax
        case ESHashbangFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESHashbang;
            break;

        // ES Symbol.prototype.description flag
        case ESSymbolDescriptionFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESSymbolDescription;
            break;

        case ESArrayFindFromLastFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESArrayFindFromLast;
            break;

        // ES Promise.any and AggregateError flag
        case ESPromiseAnyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESPromiseAny;
            break;

        // ES import.meta keyword meta-property
        case ESImportMetaFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESImportMeta;
            break;

        //ES globalThis flag
        case ESGlobalThisFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESGlobalThis;
            break;

        // This flag to be removed once JITing generator functions is stable
        case JitES6GeneratorsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_JitES6Generators;
            break;

        case FastLineColumnCalculationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_FastLineColumnCalculation;
            break;
        case FreeRejittedCodeFlag:
            retValue = (Boolean) true;
            break;
        case ForceGuardPagesFlag:
            retValue = (Boolean) false;
            break;
        case PrintGuardPageBoundsFlag:
            retValue = (Boolean) false;
            break;
        case ForceLegacyEngineFlag:
            retValue = (Boolean) false;
            break;
        case ForceArrayBTreeFlag:
            retValue = (Boolean) false;
            break;
        case StrongArraySortFlag:
            retValue = (Boolean) DEFAULT_CONFIG_StrongArraySort;
            break;
        case ForceCleanPropertyOnCollectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceCleanPropertyOnCollect;
            break;
        case ForceCleanCacheOnCollectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceCleanCacheOnCollect;
            break;
        case ForceGCAfterJSONParseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceGCAfterJSONParse;
            break;
        case ForceDecommitOnCollectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceDecommitOnCollect;
            break;
        case ForceDeferParseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceDeferParse;
            break;
        case ForceDiagnosticsModeFlag:
            retValue = (Boolean) false;
            break;
        case ForceGetWriteWatchOOMFlag:
            retValue = (Boolean) false;
            break;
        case ForcePostLowerGlobOptInstrStringFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForcePostLowerGlobOptInstrString;
            break;
        case ForceSplitScopeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceSplitScope;
            break;
        case EnumerateSpecialPropertiesInDebuggerFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnumerateSpecialPropertiesInDebugger;
            break;
        case EnableContinueAfterExceptionWrappersForHelpersFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForHelpers;
            break;
        case EnableContinueAfterExceptionWrappersForBuiltInsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForBuiltIns;
            break;
        case EnableFunctionSourceReportForHeapEnumFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableFunctionSourceReportForHeapEnum;
            break;
        case ForceDynamicProfileFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceDynamicProfile;
            break;
        case ForceES5ArrayFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceES5Array;
            break;
        case ForceAsmJsLinkFailFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceAsmJsLinkFail;
            break;
        case ForceExpireOnNonCacheCollectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceExpireOnNonCacheCollect;
            break;
        case ForceFastPathFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceFastPath;
            break;
        case ForceFloatPrefFlag:
            retValue = (Boolean) false;
            break;
        case ForceJITLoopBodyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceJITLoopBody;
            break;
        case ForceStaticInterpreterThunkFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceStaticInterpreterThunk;
            break;
        case DumpCommentsFromReferencedFilesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DumpCommentsFromReferencedFiles;
            break;
        case EnableFatalErrorOnOOMFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableFatalErrorOnOOM;
            break;

        #if defined(_M_ARM32_OR_ARM64)
        case ForceLocalsPtrFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case DeferLoadingAvailableSourceFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DeferLoadingAvailableSource;
            break;
        case ForceNativeFlag:
            retValue = (Boolean) false;
            break;
        case ForceSerializedFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceSerialized;
            break;
        case ForceStrictModeFlag:
            retValue = (Boolean) false;
            break;
        case ForceUndoDeferFlag:
            retValue = (Boolean) false;
            break;
        case ForceBlockingConcurrentCollectFlag:
            retValue = (Boolean) false;
            break;
        case FreTestDiagModeFlag:
            retValue = (Boolean) false;
            break;
        #ifdef BYTECODE_TESTING
        case MediumByteCodeLayoutFlag:
            retValue = (Boolean) false;
            break;
        case LargeByteCodeLayoutFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case GenerateByteCodeBufferReturnsCantGenerateFlag:
            retValue = (Boolean) false;
            break;
        case HighPrecisionDateFlag:
            retValue = (Boolean) DEFAULT_CONFIG_HighPrecisionDate;
            break;

        case PrimeRecyclerFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PrimeRecycler;
            break;
        case TraceEngineRefcountFlag:
            retValue = (Boolean) false;
            break;
        #if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
        case LeakStackTraceFlag:
            retValue = (Boolean) false;
            break;
        case ForceMemoryLeakFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case DumpAfterFinalGCFlag:
            retValue = (Boolean) false;
            break;
        case ForceOldDateAPIFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceOldDateAPI;
            break;

        case ForceMaxJitThreadCountFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceMaxJitThreadCount;
            break;

        case MitigateSpectreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_MitigateSpectre;
            break;

        case AddMaskingBlocksFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AddMaskingBlocks;
            break;

        case PoisonVarArrayLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonVarArrayLoad;
            break;
        case PoisonIntArrayLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonIntArrayLoad;
            break;
        case PoisonFloatArrayLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonFloatArrayLoad;
            break;
        case PoisonTypedArrayLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonTypedArrayLoad;
            break;
        case PoisonStringLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonStringLoad;
            break;
        case PoisonObjectsForLoadsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonObjectsForLoads;
            break;

        case PoisonVarArrayStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonVarArrayStore;
            break;
        case PoisonIntArrayStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonIntArrayStore;
            break;
        case PoisonFloatArrayStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonFloatArrayStore;
            break;
        case PoisonTypedArrayStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonTypedArrayStore;
            break;
        case PoisonStringStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonStringStore;
            break;
        case PoisonObjectsForStoresFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonObjectsForStores;
            break;

        case EnforceExecutionModeLimitsFlag:
            retValue = (Boolean) false;
            break;
        case EemlFlag:
            retValue = (Boolean) false;
            break;

        case NewSimpleJitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_NewSimpleJit;
            break;

        case NoDeferParseFlag:
            retValue = (Boolean) false;
            break;
        case NoLogoFlag:
            retValue = (Boolean) false;
            break;
        case OOPJITMissingOptsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_OOPJITMissingOpts;
            break;
        case CrashOnOOPJITFailureFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CrashOnOOPJITFailure;
            break;
        case OOPCFGRegistrationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_OOPCFGRegistration;
            break;
        case ForceJITCFGCheckFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceJITCFGCheck;
            break;
        case UseJITTrampolineFlag:
            retValue = (Boolean) DEFAULT_CONFIG_UseJITTrampoline;
            break;
        case NoNativeFlag:
            retValue = (Boolean) false;
            break;
        case NoStrictModeFlag:
            retValue = (Boolean) false;
            break;
        case NormalizeStatsFlag:
            retValue = (Boolean) false;
            break;
        #ifdef ENABLE_TRACE
        case InMemoryTraceFlag:
            retValue = (Boolean) DEFAULT_CONFIG_InMemoryTrace;
            break;
        #ifdef STACK_BACK_TRACE
        case TraceWithStackFlag:
            retValue = (Boolean) DEFAULT_CONFIG_TraceWithStack;
            break;
        #endif // STACK_BACK_TRACE
        #endif // ENABLE_TRACE
        case PrintRunTimeDataCollectionTraceFlag:
            retValue = (Boolean) false;
            break;
        #ifdef ENABLE_PREJIT
        case PrejitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_Prejit;
            break;
        #endif
        case PrintSrcInDumpFlag:
            retValue = (Boolean) true;
            break;
        #ifdef PROFILE_OBJECT_LITERALS
        case ProfileObjectLiteralFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #ifdef PROFILE_STRINGS
        case ProfileStringsFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #ifdef PROFILE_TYPES
        case ProfileTypesFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #ifdef PROFILE_EVALMAP
        case ProfileEvalMapFlag:
            retValue = (Boolean) false;
            break;
        #endif

        #ifdef PROFILE_BAILOUT_RECORD_MEMORY
        case ProfileBailOutRecordMemoryFlag:
            retValue = (Boolean) false;
            break;
        #endif

        #if DBG
        case ValidateIntRangesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ValidateIntRanges;
            break;
        #endif
        case LibraryStackFrameFlag:
            retValue = (Boolean) DEFAULT_CONFIG_LibraryStackFrame;
            break;
        case LibraryStackFrameDebuggerFlag:
            retValue = (Boolean) DEFAULT_CONFIG_LibraryStackFrameDebugger;
            break;
        #ifdef RECYCLER_STRESS
        case RecyclerStressFlag:
            retValue = (Boolean) false;
            break;
        #if ENABLE_CONCURRENT_GC
        case RecyclerBackgroundStressFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerConcurrentStressFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerConcurrentRepeatStressFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #if ENABLE_PARTIAL_GC
        case RecyclerPartialStressFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case RecyclerTrackStressFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerInduceFalsePositivesFlag:
            retValue = (Boolean) false;
            break;
        #endif // RECYCLER_STRESS
        case RecyclerForceMarkInteriorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RecyclerForceMarkInterior;
            break;
        #if ENABLE_CONCURRENT_GC
        case EnableConcurrentSweepAllocFlag:
            retValue = (Boolean) true;
            break;
        case ecsaFlag:
            retValue = (Boolean) true;
            break;
        #endif
        #ifdef RECYCLER_PAGE_HEAP
        case PageHeapAllocStackFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PageHeapAllocStack;
            break;
        case PageHeapFreeStackFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PageHeapFreeStack;
            break;
        case PageHeapDecommitGuardPageFlag:
            retValue = (Boolean) true;
            break;
        #endif
        #ifdef RECYCLER_NO_PAGE_REUSE
        case RecyclerNoPageReuseFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case RecyclerTestFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerProtectPagesOnRescanFlag:
            retValue = (Boolean) false;
            break;
        #ifdef RECYCLER_VERIFY_MARK
        case RecyclerVerifyMarkFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #if EXCEPTION_RECOVERY
        case SwallowExceptionsFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case PrintSystemExceptionFlag:
            retValue = (Boolean) false;
            break;


        #if DBG_DUMP || defined(RECYCLER_TRACE)
        //TraceMetaDataParsing flag with optional levels:
        //    Level 1 = interfaces only
        //    Level 2 = interfaces and methods
        //    Level 3 = interfaces, methods and parameters
        //    Level 4 = interfaces and properties
        //    Level 5 (default) = ALL
        case TraceWin8AllocationsFlag:
            retValue = (Boolean) false;
            break;
        case TraceWin8DeallocationsImmediateFlag:
            retValue = (Boolean) false;
            break;
        case PrintWin8StatsDetailedFlag:
            retValue = (Boolean) false;
            break;
        case TraceProtectPagesFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case TraceAsyncDebugCallsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_TraceAsyncDebugCalls;
            break;
        #ifdef TRACK_DISPATCH
        case TrackDispatchFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case VerboseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_Verbose;
            break;
        case UseFullNameFlag:
            retValue = (Boolean) DEFAULT_CONFIG_UseFullName;
            break;
        case Utf8Flag:
            retValue = (Boolean) false;
            break;
        case WERExceptionSupportFlag:
            retValue = (Boolean) false;
            break;
        case ExtendedErrorStackForTestHostFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ExtendedErrorStackForTestHost;
            break;
        case errorStackTraceFlag:
            retValue = (Boolean) DEFAULT_CONFIG_errorStackTrace;
            break;
        case DoHeapEnumOnEngineShutdownFlag:
            retValue = (Boolean) false;
            break;
        #ifdef HEAP_ENUMERATION_VALIDATION
        case ValidateHeapEnumFlag:
            retValue = (Boolean) false;
            break;
        #endif

        #if ENABLE_REGEX_CONFIG_OPTIONS
        //
        // Regex flags
        //
        case RegexTracingFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexTracing;
            break;
        case RegexProfileFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexProfile;
            break;
        case RegexDebugFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexDebug;
            break;
        case RegexDebugASTFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexDebugAST;
            break;
        case RegexDebugAnnotatedASTFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexDebugAnnotatedAST;
            break;
        case RegexBytecodeDebugFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexBytecodeDebug;
            break;
        case RegexOptimizeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexOptimize;
            break;
        #endif

        case OptimizeForManyInstancesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_OptimizeForManyInstances;
            break;
        case EnableArrayTypeMutationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableArrayTypeMutation;
            break;
        case EnableEvalMapCleanupFlag:
            retValue = (Boolean) true;
            break;
        #ifdef PROFILE_MEM
        case TraceObjectAllocationFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case ForceStringKeyedSimpleDictionaryTypeHandlerFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceStringKeyedSimpleDictionaryTypeHandler;
            break;
        case TypeSnapshotEnumerationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_TypeSnapshotEnumeration;
            break;
        case IsolatePrototypesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_IsolatePrototypes;
            break;
        case ChangeTypeOnProtoFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ChangeTypeOnProto;
            break;
        case ShareInlineCachesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ShareInlineCaches;
            break;
        case DisableDebugObjectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DisableDebugObject;
            break;
        case DumpHeapFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DumpHeap;
            break;
        #ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        case MemProtectHeapFlag:
            retValue = (Boolean) DEFAULT_CONFIG_MemProtectHeap;
            break;
        #endif
        #ifdef RECYCLER_STRESS
        case MemProtectHeapStressFlag:
            retValue = (Boolean) false;
            break;
        #if ENABLE_CONCURRENT_GC
        case MemProtectHeapBackgroundStressFlag:
            retValue = (Boolean) false;
            break;
        case MemProtectHeapConcurrentStressFlag:
            retValue = (Boolean) false;
            break;
        case MemProtectHeapConcurrentRepeatStressFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #if ENABLE_PARTIAL_GC
        case MemProtectHeapPartialStressFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #endif
        #ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        case FixPropsOnPathTypesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_FixPropsOnPathTypes;
            break;
        #endif

        #if defined(_M_X64)
        case ZeroMemoryWithNonTemporalStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ZeroMemoryWithNonTemporalStore;
            break;
        #endif

        #if DBG
        case InitializeInterpreterSlotsWithInvalidStackVarFlag:
            retValue = (Boolean) false;
            break;
        #endif

        case ClearInlineCachesOnCollectFlag:
            retValue = (Boolean) false;
            break;

        #ifdef IR_VIEWER
        case IRViewerFlag:
            retValue = (Boolean) false;
            break;
        #endif /* IR_VIEWER */

        case StrictWriteBarrierCheckFlag:
            retValue = (Boolean) DEFAULT_CONFIG_StrictWriteBarrierCheck;
            break;
        case WriteBarrierTestFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WriteBarrierTest;
            break;
        case ForceSoftwareWriteBarrierFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceSoftwareWriteBarrier;
            break;
        case VerifyBarrierBitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_VerifyBarrierBit;
            break;
        case EnableBGFreeZeroFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableBGFreeZero;
            break;
        case KeepRecyclerTrackDataFlag:
            retValue = (Boolean) DEFAULT_CONFIG_KeepRecyclerTrackData;
            break;

        default:
            // not found - or not a boolean flag
            Assert(false);
        }

        return retValue;
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::SetAsBoolean
    ///
    /// Set the value of a boolean flag. If the flag is a parent flag, all children flag
    //  will be set accordingly.
    ///
    ///----------------------------------------------------------------------------
    void
    ConfigFlagsTable::SetAsBoolean(Flag flag, Boolean value)
    {
        AssertMsg(this->GetFlagType(flag) == FlagBoolean, "flag not a boolean type");

        Boolean* settingAsBoolean = this->GetAsBoolean(flag);
        Assert(settingAsBoolean != nullptr);

        Output::VerboseNote(u"FLAG %s = %d\n", FlagNames[(int) flag], value);
        *settingAsBoolean = value;

        // check if parent flag
        if (this->IsParentFlag(flag))
        {
            // parent flag, will iterate through all child flags
            Flag childFlag = GetNextChildFlag(flag, /* no currentChildFlag */ InvalidFlag);
            while (childFlag != InvalidFlag)
            {
                Boolean childDefaultValue = GetDefaultValueAsBoolean(childFlag);

                // if the parent flag is TRUE, the children flag values are based on their default values
                // if the parent flag is FALSE, the children flag values are FALSE (always - as disabled)
                Boolean childValue = value == TRUE ? childDefaultValue : FALSE;

                Output::VerboseNote(u"FLAG %s = %d - setting child flag %s = %d\n", FlagNames[(int) flag], value, FlagNames[(int) childFlag], childValue);
                this->SetAsBoolean(childFlag, childValue);

                // get next child flag
                childFlag = GetNextChildFlag(flag, /* currentChildFlag */ childFlag);
            }
        }

        // Master ES6 flag to enable ALL sub ES6 features/flags
        if( flag == ES6AllFlag )
        {
            this->FlagSetCallback_ES6All(value);
        }

        // Master ES6 flag to enable Threshold ES6 features/flags
        if( flag == ES6ExperimentalFlag )
        {
            this->FlagSetCallback_ES6Experimental(value);
        }
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetParentFlag
    ///
    /// Get the parent flag corresponding to the flag, if any, otherwise returns NoParentFlag
    ///
    ///----------------------------------------------------------------------------
    Flag
    ConfigFlagsTable::GetParentFlag(Flag flag) const
    {
        Flag parentFlag = FlagParents[(int)flag];

        return parentFlag;
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetNextChildFlag
    ///
    /// Get the next child flag for a given parent flag. If no currentChildFlag, use
    /// InvalidFlag or NoParentFlag as start iterator.
    ///
    ///----------------------------------------------------------------------------
    Flag
    ConfigFlagsTable::GetNextChildFlag(Flag parentFlag, Flag currentChildFlag)  const
    {
        // start at the current+1
        int startIndex = (int)currentChildFlag + 1;

        // otherwise start from beginning
        if (currentChildFlag == InvalidFlag || currentChildFlag == NoParentFlag)
        {
            // reset the start index
            startIndex = 0;
        }

        for(int i=startIndex; i < FlagCount; i++)
        {
            Flag currentFlag = (Flag)i;
            Flag parentFlagForCurrentFlag = GetParentFlag(currentFlag);

            if(parentFlagForCurrentFlag == parentFlag)
            {
                // found a match
                return currentFlag;
            }
        }

        // no more
        return InvalidFlag;
    }

    //
    // Special overrides for flags being set
    //
    void
    ConfigFlagsTable::FlagSetCallback_ES6All(Boolean value)
    {
        // iterate through all ES6 flags - and set them explicitly (except ES6Verbose)
        Flag parentFlag = ES6Flag;

        // parent ES6 flag, will iterate through all child ES6 flags
        Flag childFlag = GetNextChildFlag(parentFlag, /* no currentChildFlag */ InvalidFlag);
        while (childFlag != InvalidFlag)
        {
            // skip verbose
            if (childFlag != ES6VerboseFlag)
            {
                Boolean childValue = value;

                Output::VerboseNote(u"FLAG %s = %d - setting child flag %s = %d\n", FlagNames[(int) parentFlag], value, FlagNames[(int) childFlag], childValue);
                this->SetAsBoolean(childFlag, childValue);
            }

            // get next child flag
            childFlag = GetNextChildFlag(parentFlag, /* currentChildFlag */ childFlag);
        }
    }

    void
    ConfigFlagsTable::FlagSetCallback_ES6Experimental(Boolean value)
    {
        if (value)
        {
            EnableExperimentalFlag();
        }
    }


    void
    ConfigFlagsTable::EnableExperimentalFlag()
    {
        std::unique_lock autocs(csExperimentalFlags);
        this->SetAsBoolean(Js::Flag::WasmSimdFlag, true);
        this->SetAsBoolean(Js::Flag::ESDynamicImportFlag, true);
        this->SetAsBoolean(Js::Flag::ES6RegExPrototypePropertiesFlag, true);
        this->SetAsBoolean(Js::Flag::ES6RegExSymbolsFlag, true);
        this->SetAsBoolean(Js::Flag::ESSharedArrayBufferFlag, true);
    }

    //
    // Configuration options
    //

    Configuration::Configuration()
    {
    }

    Configuration        Configuration::Global;


} //namespace Js
