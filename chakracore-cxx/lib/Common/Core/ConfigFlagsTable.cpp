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
        #define FLAG(type, name, ...) \
            \
            case name##Flag : \
                return reinterpret_cast<void*>(const_cast<type*>(&##name)); \

        #include "Interface/ConfigFlagsList.h"

            default:
                return NULL;
        }
    }


    void
    ConfigFlagsTable::VerboseDump()
    {
#define FLAG(type, name, ...) \
        if (IsEnabled(name##Flag)) \
        { \
            Output::Print(u"-%s", _u(#name)); \
            switch (Flag##type) \
            { \
            case FlagBoolean: \
                if (!*GetAsBoolean(name##Flag)) \
                { \
                    Output::Print(u"-"); \
                } \
                break; \
            case FlagString: \
                if (GetAsString(name##Flag) != nullptr) \
                { \
                    Output::Print(u":%s", (const char16_t*)*GetAsString(name##Flag)); \
                } \
                break; \
            case FlagNumber: \
                Output::Print(u":%d", *GetAsNumber(name##Flag)); \
                break; \
            default: \
                break; \
            }; \
            Output::Print(u"\n"); \
        }

#include "Interface/ConfigFlagsList.h"
#undef FLAG
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
        // TODO (hanhossain): ConfigFlagsList start
        // TODO (hanhossain): remove default values start
        #ifndef DEFAULT_CONFIG_BgJitDelay
        #if _M_ARM
        #define DEFAULT_CONFIG_BgJitDelay           (70)
        #else
        #define DEFAULT_CONFIG_BgJitDelay           (30)
        #endif
        #endif // DEFAULT_CONFIG_BgJitDelay
        #define DEFAULT_CONFIG_AsmJs                (true)
        #define DEFAULT_CONFIG_AsmJsEdge            (false)
        #define DEFAULT_CONFIG_AsmJsStopOnError     (false)

        #define DEFAULT_CONFIG_Wasm               (true)
        #define DEFAULT_CONFIG_WasmI64            (false)
        #if ENABLE_FAST_ARRAYBUFFER
            #define DEFAULT_CONFIG_WasmFastArray    (true)
        #else
            #define DEFAULT_CONFIG_WasmFastArray    (false)
        #endif
        #define DEFAULT_CONFIG_WasmSharedArrayVirtualBuffer (true)
        #define DEFAULT_CONFIG_WasmCheckVersion     (true)
        #define DEFAULT_CONFIG_WasmAssignModuleID   (false)
        #define DEFAULT_CONFIG_WasmIgnoreLimits     (false)
        #define DEFAULT_CONFIG_WasmFold             (true)
        #define DEFAULT_CONFIG_WasmMathExFilter     (false)
        #define DEFAULT_CONFIG_WasmIgnoreResponse   (false)
        #define DEFAULT_CONFIG_WasmMaxTableSize     (10000000)
        #define DEFAULT_CONFIG_WasmThreads          (false)
        #define DEFAULT_CONFIG_WasmMultiValue       (false)
        #define DEFAULT_CONFIG_WasmSignExtends      (true)
        #define DEFAULT_CONFIG_WasmNontrapping      (true)
        #define DEFAULT_CONFIG_WasmExperimental     (false)
        #define DEFAULT_CONFIG_BgParse              (false)
        #define DEFAULT_CONFIG_BgJitDelayFgBuffer   (0)
        #define DEFAULT_CONFIG_BgJitPendingFuncCap  (31)
        #define DEFAULT_CONFIG_CurrentSourceInfo    (true)
        #define DEFAULT_CONFIG_CreateFunctionProxy  (true)
        #define DEFAULT_CONFIG_HybridFgJit          (false)
        #define DEFAULT_CONFIG_HybridFgJitBgQueueLengthThreshold (32)
        #define DEFAULT_CONFIG_Prejit               (false)
        #define DEFAULT_CONFIG_ParserStateCache     (true)
        #define DEFAULT_CONFIG_CompressParserStateCache (false)
        #define DEFAULT_CONFIG_DeferTopLevelTillFirstCall (true)
        #define DEFAULT_CONFIG_DirectCallTelemetryStats (false)
        #define DEFAULT_CONFIG_errorStackTrace      (true)
        #define DEFAULT_CONFIG_FastLineColumnCalculation (true)
        #define DEFAULT_CONFIG_PrintLineColumnInfo (false)
        #define DEFAULT_CONFIG_ForceDecommitOnCollect (false)
        #define DEFAULT_CONFIG_ForceDeferParse      (false)
        #define DEFAULT_CONFIG_NoDeferParse         (false)
        #define DEFAULT_CONFIG_ForceDynamicProfile  (false)
        #define DEFAULT_CONFIG_ForceExpireOnNonCacheCollect (false)
        #define DEFAULT_CONFIG_ForceFastPath        (false)
        #define DEFAULT_CONFIG_ForceJITLoopBody     (false)
        #define DEFAULT_CONFIG_ForceStaticInterpreterThunk (false)
        #define DEFAULT_CONFIG_ForceCleanPropertyOnCollect (false)
        #define DEFAULT_CONFIG_ForceCleanCacheOnCollect (false)
        #define DEFAULT_CONFIG_ForceGCAfterJSONParse (false)
        #define DEFAULT_CONFIG_ForceSerialized      (false)
        #define DEFAULT_CONFIG_ForceES5Array        (false)
        #define DEFAULT_CONFIG_ForceAsmJsLinkFail   (false)
        #define DEFAULT_CONFIG_StrongArraySort      (false)
        #define DEFAULT_CONFIG_DumpCommentsFromReferencedFiles (false)
        #define DEFAULT_CONFIG_ExtendedErrorStackForTestHost (false)
        #define DEFAULT_CONFIG_ForceSplitScope      (false)
        #define DEFAULT_CONFIG_DelayFullJITSmallFunc (0)
        #define DEFAULT_CONFIG_EnableFatalErrorOnOOM (true)
        #define DEFAULT_CONFIG_RedeferralCap         (3)

        //Following determines inline thresholds
        #define DEFAULT_CONFIG_InlineThreshold      (35)            //Default start
        #define DEFAULT_CONFIG_AggressiveInlineThreshold  (80)      //Limit for aggressive inlining.
        #define DEFAULT_CONFIG_InlineThresholdAdjustCountInLargeFunction  (20)
        #define DEFAULT_CONFIG_InlineThresholdAdjustCountInMediumSizedFunction  (6)
        #define DEFAULT_CONFIG_InlineThresholdAdjustCountInSmallFunction  (10)
        #define DEFAULT_CONFIG_ConstructorInlineThreshold (21)      //Monomorphic constructor threshold
        #define DEFAULT_CONFIG_AsmJsInlineAdjust (35)                // wasm functions are cheaper to inline, so worth being more aggressive
        #define DEFAULT_CONFIG_ConstructorCallsRequiredToFinalizeCachedType (2)
        #define DEFAULT_CONFIG_OutsideLoopInlineThreshold (16)      //Threshold to inline outside loops
        #define DEFAULT_CONFIG_LeafInlineThreshold  (60)            //Inlinee threshold for function which is leaf (irrespective of it has loops or not)
        #define DEFAULT_CONFIG_LoopInlineThreshold  (25)            //Inlinee threshold for function with loops
        #define DEFAULT_CONFIG_PolymorphicInlineThreshold  (35)     //Polymorphic inline threshold
        #define DEFAULT_CONFIG_InlineCountMax       (1200)          //Max sum of bytecodes of inlinees inlined into a function (excluding built-ins)
        #define DEFAULT_CONFIG_InlineCountMaxInLoopBodies (500)     // Max sum of bytecodes of inlinees that can be inlined into a jitted loop body (excluding built-ins)
        #define DEFAULT_CONFIG_AggressiveInlineCountMax       (8000)          //Max sum of bytecodes of inlinees inlined into a function (excluding built-ins) when inlined aggressively
        #define DEFAULT_CONFIG_MaxFuncInlineDepth   (2)             //Maximum number of times a function can be inlined within a top function
        #define DEFAULT_CONFIG_MaxNumberOfInlineesWithLoop   (40) //Inlinee with a loop is controlled by LoopInlineThreshold, though we don't want to inline lot of inlinees with loop, this ensures a limit.
        #define DEFAULT_CONFIG_ConstantArgumentInlineThreshold   (157)      // Bytecode threshold for functions with constant arguments which are used for branching
        #define DEFAULT_CONFIG_RecursiveInlineThreshold     (2000)      // Bytecode threshold recursive call at a call site
        #define DEFAULT_CONFIG_RecursiveInlineDepthMax      (8)      // Maximum inline depth for recursive calls
        #define DEFAULT_CONFIG_RecursiveInlineDepthMin      (2)      // Minimum inline depth for recursive call
        #define DEFAULT_CONFIG_InlineInLoopBodyScaleDownFactor    (4)
        #define DEFAULT_CONFIG_PropertyCacheMissPenalty (10)
        #define DEFAULT_CONFIG_PropertyCacheMissThreshold (-100)
        #define DEFAULT_CONFIG_PropertyCacheMissReset (-5000)

        #define DEFAULT_CONFIG_CloneInlinedPolymorphicCaches (true)
        #define DEFAULT_CONFIG_HighPrecisionDate    (false)
        #define DEFAULT_CONFIG_ForceOldDateAPI      (false)
        #define DEFAULT_CONFIG_Loop                 (1)
        #define DEFAULT_CONFIG_ForceDiagnosticsMode (false)
        #define DEFAULT_CONFIG_UseFullName          (true)
        #define DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForHelpers  (true)
        #define DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForBuiltIns  (true)
        #define DEFAULT_CONFIG_EnableFunctionSourceReportForHeapEnum (true)
        #define DEFAULT_CONFIG_LoopInterpretCount   (150)
        #define DEFAULT_CONFIG_LoopProfileIterations (25)
        #define DEFAULT_CONFIG_JitLoopBodyHotLoopThreshold (20000)
        #define DEFAULT_CONFIG_LoopBodySizeThresholdToDisableOpts (255)

        #define DEFAULT_CONFIG_MaxJitThreadCount        (2)
        #define DEFAULT_CONFIG_ForceMaxJitThreadCount   (false)

        #define DEFAULT_CONFIG_MitigateSpectre (true)

        #define DEFAULT_CONFIG_AddMaskingBlocks (true)

        #define DEFAULT_CONFIG_PoisonVarArrayLoad (true)
        #define DEFAULT_CONFIG_PoisonIntArrayLoad (true)
        #define DEFAULT_CONFIG_PoisonFloatArrayLoad (true)
        #define DEFAULT_CONFIG_PoisonTypedArrayLoad (true)
        #define DEFAULT_CONFIG_PoisonStringLoad (true)
        #define DEFAULT_CONFIG_PoisonObjectsForLoads (true)

        #define DEFAULT_CONFIG_PoisonVarArrayStore (true)
        #define DEFAULT_CONFIG_PoisonIntArrayStore (true)
        #define DEFAULT_CONFIG_PoisonFloatArrayStore (true)
        #define DEFAULT_CONFIG_PoisonTypedArrayStore (true)
        #define DEFAULT_CONFIG_PoisonStringStore (true)
        #define DEFAULT_CONFIG_PoisonObjectsForStores (true)

        #ifdef RECYCLER_PAGE_HEAP
        #define DEFAULT_CONFIG_PageHeap             ((Js::Number) PageHeapMode::PageHeapModeOff)
        #define DEFAULT_CONFIG_PageHeapAllocStack   (false)
        #define DEFAULT_CONFIG_PageHeapFreeStack    (false)
        #define DEFAULT_CONFIG_PageHeapBlockType    ((Js::Number) PageHeapBlockTypeFilter::PageHeapBlockTypeFilterAll)
        #endif

        #define DEFAULT_CONFIG_LowMemoryCap         (0xB900000) // 185 MB - based on memory cap for process on low-capacity device
        #define DEFAULT_CONFIG_NewPagesCapDuringBGSweeping    (15000 * 4)
        #define DEFAULT_CONFIG_MaxSingleAllocSizeInMB  (2048)
        #define DEFAULT_CONFIG_AllocationPolicyLimit    (-1)

        #define DEFAULT_CONFIG_MaxCodeFill          (500)
        #define DEFAULT_CONFIG_MaxLoopsPerFunction  (10)
        #define DEFAULT_CONFIG_NopFrequency         (8)
        #define DEFAULT_CONFIG_SpeculationCap       (1)         // Needs to be 1 and not 0 since the compiler complains about a condition being always false
        #define DEFAULT_CONFIG_ProfileBasedSpeculationCap (1600)
        #define DEFAULT_CONFIG_Verbose              (false)
        #define DEFAULT_CONFIG_ForceStrictMode      (false)
        #define DEFAULT_CONFIG_EnableEvalMapCleanup (true)
        #define DEFAULT_CONFIG_ExpirableCollectionGCCount (5)  // Number of GCs during which entry point profiling occurs
        #define DEFAULT_CONFIG_ExpirableCollectionTriggerThreshold (50)  // Threshold at which Entry Point Collection is triggered
        #define DEFAULT_CONFIG_RegexTracing         (false)
        #define DEFAULT_CONFIG_RegexProfile         (false)
        #define DEFAULT_CONFIG_RegexDebug           (false)
        #define DEFAULT_CONFIG_RegexDebugAST        (true)
        #define DEFAULT_CONFIG_RegexDebugAnnotatedAST (true)
        #define DEFAULT_CONFIG_RegexBytecodeDebug   (false)
        #define DEFAULT_CONFIG_RegexOptimize        (true)
        #define DEFAULT_CONFIG_DynamicRegexMruListSize (16)
        #define DEFAULT_CONFIG_GoptCleanupThreshold  (25)
        #define DEFAULT_CONFIG_AsmGoptCleanupThreshold  (500)
        #define DEFAULT_CONFIG_OptimizeForManyInstances (false)
        #define DEFAULT_CONFIG_EnableArrayTypeMutation (false)

        #define DEFAULT_CONFIG_DeferParseThreshold             (4 * 1024) // Unit is number of characters
        #define DEFAULT_CONFIG_ProfileBasedDeferParseThreshold (100)      // Unit is number of characters

        #define DEFAULT_CONFIG_ProfileBasedSpeculativeJit (true)
        #define DEFAULT_CONFIG_WininetProfileCache        (true)
        #define DEFAULT_CONFIG_MinProfileCacheSize        (5)   // Minimum number of functions before profile is saved.
        #define DEFAULT_CONFIG_ProfileDifferencePercent   (15)  // If 15% of the functions have different profile we will trigger a save.

        #define DEFAULT_CONFIG_Intl                    (true)
        #define DEFAULT_CONFIG_IntlBuiltIns            (true)
        #define DEFAULT_CONFIG_IntlPlatform            (false) // Makes the EngineExtension.Intl object visible to user code as Intl.platform, meant for testing

        #ifdef ENABLE_JS_BUILTINS
            #define DEFAULT_CONFIG_JsBuiltIn             (true)
        #else
            #define DEFAULT_CONFIG_JsBuiltIn             (false)
        #endif
        #define DEFAULT_CONFIG_JitRepro                (false)
        #define DEFAULT_CONFIG_LdChakraLib             (false)
        #define DEFAULT_CONFIG_TestChakraLib           (false)
        #define DEFAULT_CONFIG_EntryPointInfoRpcData   (false)

        // ES6 DEFAULT BEHAVIOR
        #define DEFAULT_CONFIG_ES6                     (true)  // master flag to gate all P0-spec-test compliant ES6 features

        //CollectGarbage is legacy IE specific global function disabled in Microsoft Edge.
        #define DEFAULT_CONFIG_CollectGarbage          (false)

        // ES6 sub-feature gate - to enable-disable ES6 sub-feature when ES6 flag is enabled
        #define DEFAULT_CONFIG_ES6DateParseFix         (true)
        #define DEFAULT_CONFIG_ES6FunctionNameFull     (true)
        #define DEFAULT_CONFIG_ES6Generators           (true)
        #define DEFAULT_CONFIG_ES6IsConcatSpreadable   (true)
        #define DEFAULT_CONFIG_ES6Math                 (true)
        #ifdef COMPILE_DISABLE_ES6Module
            // If ES6Module needs to be disabled by compile flag, DEFAULT_CONFIG_ES6Module should be false
            #define DEFAULT_CONFIG_ES6Module               (false)
        #else
            #define DEFAULT_CONFIG_ES6Module               (true)
        #endif
        #define DEFAULT_CONFIG_ES6Object               (true)
        #define DEFAULT_CONFIG_ES6Number               (true)
        #define DEFAULT_CONFIG_ES6ObjectLiterals       (true)
        #define DEFAULT_CONFIG_ES6Proxy                (true)
        #define DEFAULT_CONFIG_ES6Rest                 (true)
        #define DEFAULT_CONFIG_ES6Spread               (true)
        #define DEFAULT_CONFIG_ES6String               (true)
        #define DEFAULT_CONFIG_ES6StringPrototypeFixes (true)
        #define DEFAULT_CONFIG_ES2018ObjectRestSpread  (true)

        #ifndef DEFAULT_CONFIG_ES6PrototypeChain
        #ifdef COMPILE_DISABLE_ES6PrototypeChain
            // If ES6PrototypeChain needs to be disabled by compile flag, DEFAULT_CONFIG_ES6PrototypeChain should be false
            #define DEFAULT_CONFIG_ES6PrototypeChain       (false)
        #else
            #define DEFAULT_CONFIG_ES6PrototypeChain       (true)
        #endif
        #endif

        #define DEFAULT_CONFIG_ES6ToPrimitive          (true)
        #define DEFAULT_CONFIG_ES6ToLength             (true)
        #define DEFAULT_CONFIG_ES6ToStringTag          (true)
        #define DEFAULT_CONFIG_ES6Unicode              (true)
        #define DEFAULT_CONFIG_ES6UnicodeVerbose       (true)
        #define DEFAULT_CONFIG_ES6Unscopables          (true)
        #define DEFAULT_CONFIG_ES6RegExSticky          (true)
        #define DEFAULT_CONFIG_ES2018RegExDotAll       (true)
        #define DEFAULT_CONFIG_ESBigInt                (false)
        #define DEFAULT_CONFIG_ESNumericSeparator      (true)
        #define DEFAULT_CONFIG_ESHashbang              (true)
        #define DEFAULT_CONFIG_ESSymbolDescription     (true)
        #define DEFAULT_CONFIG_ESArrayFindFromLast     (true)
        #define DEFAULT_CONFIG_ESPromiseAny            (true)
        #define DEFAULT_CONFIG_ESNullishCoalescingOperator (true)
        #define DEFAULT_CONFIG_ESGlobalThis            (true)

        // Jitting generator functions is not functional on ARM
        // Also still contains significant bugs on x86/x64 hence disabled
        #ifdef _M_ARM32_OR_ARM64
            #define DEFAULT_CONFIG_JitES6Generators            (false)
        #else
            #define DEFAULT_CONFIG_JitES6Generators            (false)
        #endif

        #ifdef COMPILE_DISABLE_ES6RegExPrototypeProperties
            // If ES6RegExPrototypeProperties needs to be disabled by compile flag, DEFAULT_CONFIG_ES6RegExPrototypeProperties should be false
            #define DEFAULT_CONFIG_ES6RegExPrototypeProperties (false)
        #else
            #define DEFAULT_CONFIG_ES6RegExPrototypeProperties (false)
        #endif
        #ifdef COMPILE_DISABLE_ES6RegExSymbols
            // If ES6RegExSymbols needs to be disabled by compile flag, DEFAULT_CONFIG_ES6RegExSymbols should be false
            #define DEFAULT_CONFIG_ES6RegExSymbols         (false)
        #else
            #define DEFAULT_CONFIG_ES6RegExSymbols         (false)
        #endif
        #define DEFAULT_CONFIG_ES7AsyncAwait           (true)
        #define DEFAULT_CONFIG_ES7ExponentionOperator  (true)
        #define DEFAULT_CONFIG_ES7TrailingComma        (true)
        #define DEFAULT_CONFIG_ES7ValuesEntries        (true)
        #define DEFAULT_CONFIG_ESObjectGetOwnPropertyDescriptors (true)
        #define DEFAULT_CONFIG_ESDynamicImport         (true)
        #define DEFAULT_CONFIG_ESImportMeta            (true)
        #define DEFAULT_CONFIG_ESExportNsAs            (true)
        #define DEFAULT_CONFIG_ES2018AsyncIteration    (true)
        #define DEFAULT_CONFIG_ESTopLevelAwait         (true)

        #define DEFAULT_CONFIG_ESSharedArrayBuffer     (false)

        #define DEFAULT_CONFIG_ES6Verbose              (false)
        #define DEFAULT_CONFIG_ES6All                  (false)
        // ES6 DEFAULT BEHAVIOR

        #define DEFAULT_CONFIG_AsyncDebugging           (true)
        #define DEFAULT_CONFIG_TraceAsyncDebugCalls     (false)
        #define DEFAULT_CONFIG_ForcePostLowerGlobOptInstrString (false)
        #define DEFAULT_CONFIG_EnumerateSpecialPropertiesInDebugger (true)

        #define DEFAULT_CONFIG_MaxJITFunctionBytecodeByteLength (4800000)
        #define DEFAULT_CONFIG_MaxJITFunctionBytecodeCount (120000)

        #define DEFAULT_CONFIG_JitQueueThreshold      (6)

        #define DEFAULT_CONFIG_FullJitRequeueThreshold (25)     // Minimum number of times a function needs to be executed before it is re-added to the jit queue

        #define DEFAULT_CONFIG_MinTemplatizedJitRunCount      (100)     // Minimum number of times a function needs to be interpreted before it is jitted
        #define DEFAULT_CONFIG_MinAsmJsInterpreterRunCount      (10)     // Minimum number of times a function needs to be Asm interpreted before it is jitted
        #define DEFAULT_CONFIG_MinTemplatizedJitLoopRunCount      (500)     // Minimum number of times a function needs to be interpreted before it is jitted
        #define DEFAULT_CONFIG_MaxTemplatizedJitRunCount      (-1)     // Maximum number of times a function can be TJ before it is jitted
        #define DEFAULT_CONFIG_MaxAsmJsInterpreterRunCount      (-1)     // Maximum number of times a function can be Asm interpreted before it is jitted

        // Note: The following defaults only apply when the NewSimpleJit is on. The defaults for when it's off are computed in
        // ConfigFlagsTable::TranslateFlagConfiguration.
        #define DEFAULT_CONFIG_AutoProfilingInterpreter0Limit (12)
        #define DEFAULT_CONFIG_ProfilingInterpreter0Limit (4)
        #define DEFAULT_CONFIG_AutoProfilingInterpreter1Limit (0)
        #define DEFAULT_CONFIG_SimpleJitLimit (132)
        #define DEFAULT_CONFIG_ProfilingInterpreter1Limit (12)

        // These are used to compute the above defaults for when NewSimpleJit is off
        #define DEFAULT_CONFIG_AutoProfilingInterpreterLimit_OldSimpleJit (80)
        #define DEFAULT_CONFIG_SimpleJitLimit_OldSimpleJit (25)

        #define DEFAULT_CONFIG_MinProfileIterations (16)
        #define DEFAULT_CONFIG_MinProfileIterations_OldSimpleJit (25)
        #define DEFAULT_CONFIG_MinSimpleJitIterations (16)
        #define DEFAULT_CONFIG_NewSimpleJit (false)

        #define DEFAULT_CONFIG_MaxLinearIntCaseCount     (3)       // Maximum number of cases (in switch statement) for which instructions can be generated linearly.
        #define DEFAULT_CONFIG_MaxSingleCharStrJumpTableRatio  (2)       // Maximum single char string jump table size as multiples of the actual case arm
        #define DEFAULT_CONFIG_MaxSingleCharStrJumpTableSize  (128)       // Maximum single char string jump table size
        #define DEFAULT_CONFIG_MinSwitchJumpTableSize   (9)     // Minimum number of case target entries in the jump table(this may also include values that are missing in the consecutive set of integer case arms)
        #define DEFAULT_CONFIG_SwitchOptHolesThreshold  (50)     // Maximum percentage of holes (missing case values in a switch statement) with which a jump table can be created
        #define DEFAULT_CONFIG_MaxLinearStringCaseCount (4)     // Maximum number of String cases (in switch statement) for which instructions can be generated linearly.

        #define DEFAULT_CONFIG_MinDeferredFuncTokenCount (20)   // Minimum size in tokens of a defer-parsed function

        #if DBG
        #define DEFAULT_CONFIG_SkipFuncCountForBailOnNoProfile (0) //Initial Number of functions in a func body to be skipped from forcibly inserting BailOnNoProfile.
        #endif
        #define DEFAULT_CONFIG_BailOnNoProfileLimit    200      // The limit of bailout on no profile info before triggering a rejit
        #define DEFAULT_CONFIG_BailOnNoProfileRejitLimit (50)   // The limit of bailout on no profile info before disable all the no profile bailouts
        #define DEFAULT_CONFIG_CallsToBailoutsRatioForRejit 10   // Ratio of function calls to bailouts above which a rejit is considered
        #define DEFAULT_CONFIG_LoopIterationsToBailoutsRatioForRejit 50 // Ratio of loop iteration count to bailouts above which a rejit of the loop body is considered
        #define DEFAULT_CONFIG_MinBailOutsBeforeRejit 2         // Minimum number of bailouts for a single bailout record after which a rejit is considered
        #define DEFAULT_CONFIG_MinBailOutsBeforeRejitForLoops 2         // Minimum number of bailouts for a single bailout record after which a rejit is considered
        #define DEFAULT_CONFIG_RejitMaxBailOutCount 500         // Maximum number of bailouts for a single bailout record after which rejit is forced.

        #if DBG
        #define DEFAULT_CONFIG_ValidateIntRanges (false)
        #endif

        #define DEFAULT_CONFIG_Sse                  (-1)

        #define DEFAULT_CONFIG_DeletedPropertyReuseThreshold (32)
        #define DEFAULT_CONFIG_BigDictionaryTypeHandlerThreshold (0xffff)
        #define DEFAULT_CONFIG_ForceStringKeyedSimpleDictionaryTypeHandler (false)
        #define DEFAULT_CONFIG_TypeSnapshotEnumeration (true)
        #define DEFAULT_CONFIG_ConcurrentRuntime (false)
        #define DEFAULT_CONFIG_PrimeRecycler     (false)
        #define DEFAULT_CONFIG_DisableRentalThreading (false)
        #define DEFAULT_CONFIG_DisableDebugObject (false)
        #define DEFAULT_CONFIG_DumpHeap (false)
        #define DEFAULT_CONFIG_PerfHintLevel (1)
        #define DEFAULT_CONFIG_OOPJITMissingOpts (false)
        #define DEFAULT_CONFIG_OOPCFGRegistration (true)
        #define DEFAULT_CONFIG_CrashOnOOPJITFailure (false)
        #define DEFAULT_CONFIG_ForceJITCFGCheck (false)
        #define DEFAULT_CONFIG_UseJITTrampoline (true)

        #define DEFAULT_CONFIG_IsolatePrototypes    (true)
        #define DEFAULT_CONFIG_ChangeTypeOnProto    (true)
        #define DEFAULT_CONFIG_FixPropsOnPathTypes    (true)
        #define DEFAULT_CONFIG_BailoutTraceFilter (-1)
        #define DEFAULT_CONFIG_TempMin    (0)
        #define DEFAULT_CONFIG_TempMax    (INT_MAX)

        #define DEFAULT_CONFIG_LibraryStackFrame            (true)
        #define DEFAULT_CONFIG_LibraryStackFrameDebugger    (false)

        #define DEFAULT_CONFIG_FuncObjectInlineCacheThreshold   (2) // Maximum number of inline caches a function body may have to allow for inline caches to be allocated on the function object.
        #define DEFAULT_CONFIG_ShareInlineCaches (false)
        #define DEFAULT_CONFIG_InlineCacheInvalidationListCompactionThreshold (4)
        #define DEFAULT_CONFIG_ConstructorCacheInvalidationThreshold (500)

        #define DEFAULT_CONFIG_InMemoryTrace                (false)
        #define DEFAULT_CONFIG_InMemoryTraceBufferSize      (1024)
        #define DEFAULT_CONFIG_RichTraceFormat              (false)
        #define DEFAULT_CONFIG_TraceWithStack               (false)

        #define DEFAULT_CONFIG_InjectPartiallyInitializedInterpreterFrameError (0)
        #define DEFAULT_CONFIG_InjectPartiallyInitializedInterpreterFrameErrorType (0)

        #define DEFAULT_CONFIG_DeferLoadingAvailableSource  (false)

        #define DEFAULT_CONFIG_RecyclerForceMarkInterior (false)

        #define DEFAULT_CONFIG_MemProtectHeap (false)

        #define DEFAULT_CONFIG_InduceCodeGenFailure (30) // When -InduceCodeGenFailure is passed in, 30% of JIT allocations will fail

        #define DEFAULT_CONFIG_SkipSplitWhenResultIgnored (false)

        #define DEFAULT_CONFIG_MinMemOpCount (16U)

        #if ENABLE_COPYONACCESS_ARRAY
        #define DEFAULT_CONFIG_MaxCopyOnAccessArrayLength (32U)
        #define DEFAULT_CONFIG_MinCopyOnAccessArrayLength (5U)
        #define DEFAULT_CONFIG_CopyOnAccessArraySegmentCacheSize (16U)
        #endif

        #if defined(_M_X64)
        #define DEFAULT_CONFIG_LoopAlignNopLimit (6)
        #endif

        #if defined(_M_X64)
        #define DEFAULT_CONFIG_ZeroMemoryWithNonTemporalStore (true)
        #endif

        #define DEFAULT_CONFIG_StrictWriteBarrierCheck  (false)
        #define DEFAULT_CONFIG_KeepRecyclerTrackData  (false)
        #define DEFAULT_CONFIG_EnableBGFreeZero (true)

        #if !GLOBAL_ENABLE_WRITE_BARRIER
        #define DEFAULT_CONFIG_ForceSoftwareWriteBarrier  (false)
        #else
        #define DEFAULT_CONFIG_ForceSoftwareWriteBarrier  (true)
        #endif
        #define DEFAULT_CONFIG_WriteBarrierTest (false)
        #define DEFAULT_CONFIG_VerifyBarrierBit  (false)

        #define TraceLevel_Error        (1)
        #define TraceLevel_Warning      (2)
        #define TraceLevel_Info         (3)

        #define TEMP_ENABLE_FLAG_FOR_APPX_BETA_ONLY 1

        #define INMEMORY_CACHE_MAX_URL                    (5)             // This is the max number of URLs that the in-memory profile cache can hold.
        #define INMEMORY_CACHE_MAX_PROFILE_MANAGER        (50)            // This is the max number of dynamic scripts that the in-memory profile cache can have

        #ifdef SUPPORT_INTRUSIVE_TESTTRACES
        #define INTRUSIVE_TESTTRACE_PolymorphicInlineCache (1)
        #endif

        // TODO (hanhossain): remove default values end

        //
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

        // Please keep this list alphabetically sorted

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

        // TODO (hanhossain): ConfigFlagsList end

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
