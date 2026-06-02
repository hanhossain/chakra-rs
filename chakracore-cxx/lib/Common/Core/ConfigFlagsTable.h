//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <mutex>

#include "DataStructures/BaseDictionary.h"
#include "DataStructures/SList.h"
#include "Memory/HeapAllocator.h"

namespace Js
{
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// enum FlagTypes
    ///
    /// Different types of flags supported by the debug table
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    enum FlagTypes
    {
        InvalidFlagType,
        FlagString,
        FlagPhases,
        FlagNumber,
        FlagBoolean,
        FlagNumberSet,
        FlagNumberPairSet,
        FlagNumberTrioSet,
        FlagNumberRange
    };

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// enum Flag
    ///
    /// All the flags available. The list of flags are described in the file
    /// ConfigFlagsList.h
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    enum Flag: unsigned short
    {
#define FLAG(type, name, ...) name##Flag,

#include "Interface/ConfigFlagsList.h"
        FlagCount,
        InvalidFlag,
        NoParentFlag,
        FlagMax = NoParentFlag
    } ;


    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// enum Phase
    ///
    /// Different Phases/Subphases of the backend. The list of phases is described
    /// in the file ConfigFlagsList.h
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    enum Phase: unsigned short
    {
    AllPhase,
        BGJitPhase,
        ModulePhase,
        LibInitPhase,
            JsLibInitPhase,
        ParsePhase,
            RegexCompilePhase,
            DeferParsePhase,
            RedeferralPhase,
            DeferEventHandlersPhase,
            FunctionSourceInfoParsePhase,
            StringTemplateParsePhase,
            CreateParserStatePhase,
            SkipNestedDeferredPhase,
            CacheScopeInfoNamesPhase,
            ScanAheadPhase,
            ParallelParsePhase,
            EarlyReferenceErrorsPhase,
            EarlyErrorOnAssignToCallPhase,
            BgParsePhase,
        ByteCodePhase,
            CachedScopePhase,
            StackFuncPhase,
            StackClosurePhase,
            DisableStackFuncOnDeferredEscapePhase,
            DelayCapturePhase,
            DebuggerScopePhase,
            ByteCodeSerializationPhase,
                VariableIntEncodingPhase,
            NativeCodeSerializationPhase,
            OptimizeBlockScopePhase,
        DelayPhase,
            SpeculationPhase,
            GatherCodeGenDataPhase,
        WasmPhase,
            // Wasm frontend
            WasmBytecodePhase, // Supports -off,-dump,-trace,-profile
            WasmReaderPhase, // Support -trace,-profile
            WasmSectionPhase, // Supports -trace
            WasmOpCodeDistributionPhase, // Support -dump
            // Wasm features per functions
            WasmDeferredPhase,
            WasmValidatePrejitPhase,
            WasmInOutPhase, // Trace input and output of wasm calls
            WasmMemWritesPhase, // Trace memory writes
        AsmjsPhase,
            AsmjsTmpRegisterAllocationPhase,
            AsmjsEncoderPhase,
            AsmjsInterpreterPhase,
            AsmJsJITTemplatePhase,
            AsmjsFunctionEntryPhase,
            AsmjsInterpreterStackPhase,
            AsmjsEntryPointInfoPhase,
            AsmjsCallDebugBreakPhase,
        BackEndPhase,
            IRBuilderPhase,
                SwitchOptPhase,
                BailOnNoProfilePhase,
                BackendConcatExprOptPhase,
                ClosureRangeCheckPhase,
                ClosureRegCheckPhase,
            InlinePhase,
                InlineRecursivePhase,
                InlineAtEveryCallerPhase,      //Inlines a function, say, foo at every caller of foo. Doesn't guarantee all the calls within foo are inlined too.
                InlineTreePhase,               //Inlines every function within a top function, say, foo (which needs to be top function) Note: -force:inline achieves the effect of both -force:InlineTree & -force:InlineAtEveryCaller
                TryAggressiveInliningPhase,
                InlineConstructorsPhase,
                InlineBuiltInPhase,
                InlineInJitLoopBodyPhase,
                InlineAccessorsPhase,
                InlineGettersPhase,
                InlineSettersPhase,
                InlineApplyPhase,
                InlineApplyTargetPhase,
                InlineApplyWithoutArrayArgPhase,
                InlineAnyCallApplyTargetPhase,
                BailOutOnNotStackArgsPhase,
                InlineCallPhase,
                InlineCallTargetPhase,
                PartialPolymorphicInlinePhase,
                PolymorphicInlinePhase,
                PolymorphicInlineFixedMethodsPhase,
                InlineOutsideLoopsPhase,
                InlineFunctionsWithLoopsPhase,
                EliminateArgoutForInlineePhase,
                InlineBuiltInCallerPhase,
                InlineArgsOptPhase,
                    RemoveInlineFramePhase,
                InlinerConstFoldPhase,
                InlineCallbacksPhase,
        ExecBOIFastPathPhase,
            FGBuildPhase,
                OptimizeTryFinallyPhase,
                RemoveBreakBlockPhase,
                TailDupPhase,
            FGPeepsPhase,
            GlobOptPhase,
                PathDepBranchFoldingPhase,
                OptimizeTryCatchPhase,
                CaptureByteCodeRegUsePhase,
                BackwardPhase,
                    TrackIntUsagePhase,
                    TrackNegativeZeroPhase,
                    TypedArrayVirtualPhase,
                    TrackIntOverflowPhase,
                    TrackCompoundedIntOverflowPhase,
                ForwardPhase,
                    ValueTablePhase,
                    ValueNumberingPhase,
                    AVTInPrePassPhase,
                    PathDependentValuesPhase,
                        TrackRelativeIntBoundsPhase,
                            BoundCheckEliminationPhase,
                                BoundCheckHoistPhase,
                                    LoopCountBasedBoundCheckHoistPhase,
                    CopyPropPhase,
                        ObjPtrCopyPropPhase,
                    ConstPropPhase,
                    Int64ConstPropPhase,
                    ConstFoldPhase,
                    CSEPhase,
                    HoistConstIntPhase,
                    TypeSpecPhase,
                    AggressiveIntTypeSpecPhase,
                    AggressiveMulIntTypeSpecPhase,
                    LossyIntTypeSpecPhase,
                    FloatTypeSpecPhase,
                    StringTypeSpecPhase,
                    InductionVarsPhase,
                    InvariantsPhase,
                    FieldCopyPropPhase,
                    FieldPREPhase,
                    MakeObjSymLiveInLandingPadPhase,
                    HostOptPhase,
                    ObjTypeSpecPhase,
                        ObjTypeSpecNewObjPhase,
                        ObjTypeSpecIsolatedFldOpsPhase,
                        ObjTypeSpecIsolatedFldOpsWithBailOutPhase,
                        ObjTypeSpecStorePhase,
                        EquivObjTypeSpecPhase,
                        EquivObjTypeSpecByDefaultPhase,
                        TraceObjTypeSpecTypeGuardsPhase,
                        TraceObjTypeSpecWriteGuardsPhase,
                        LiveOutFieldsPhase,
                        DisabledObjTypeSpecPhase,
                        DepolymorphizeInlineesPhase,
                        ReuseAuxSlotPtrPhase,
                        PolyEquivTypeGuardPhase,
                        DeadStoreTypeChecksOnStoresPhase,
                        #if DBG
                            SimulatePolyCacheWithOneTypeForFunctionPhase,
                        #endif
                    CheckThisPhase,
                    StackArgOptPhase,
                    StackArgFormalsOptPhase,
                    StackArgLenConstOptPhase,
                    IndirCopyPropPhase,
                    ArrayCheckHoistPhase,
                        ArrayMissingValueCheckHoistPhase,
                        ArraySegmentHoistPhase,
                            JsArraySegmentHoistPhase,
                        ArrayLengthHoistPhase,
                        EliminateArrayAccessHelperCallPhase,
                    NativeArrayPhase,
                        NativeNewScArrayPhase,
                        NativeArrayConversionPhase,
                        CopyOnAccessArrayPhase,
                        NativeArrayLeafSegmentPhase,
                    TypedArrayTypeSpecPhase,
                    LdLenIntSpecPhase,
                    FixDataPropsPhase,
                    FixMethodPropsPhase,
                    FixAccessorPropsPhase,
                    FixDataVarPropsPhase,
                    UseFixedDataPropsPhase,
                    UseFixedDataPropsInInlinerPhase,
                    LazyBailoutPhase,
                        LazyBailoutOnImplicitCallsPhase,
                        LazyFixedDataBailoutPhase,
                        LazyFixedTypeBailoutPhase,
                    FixedMethodsPhase,
                        FEFixedMethodsPhase,
                        FixedFieldGuardCheckPhase,
                        FixedNewObjPhase,
                            JitAllocNewObjPhase,
                        FixedCtorInliningPhase,
                        FixedCtorCallsPhase,
                        FixedScriptMethodInliningPhase,
                        FixedScriptMethodCallsPhase,
                        FixedBuiltInMethodInliningPhase,
                        FixedBuiltInMethodCallsPhase,
                        SplitNewScObjectPhase,
                    OptTagChecksPhase,
                    MemOpPhase,
                        MemSetPhase,
                        MemCopyPhase,
                    IncrementalBailoutPhase,
                DeadStorePhase,
                    ReverseCopyPropPhase,
                    MarkTempPhase,
                        MarkTempNumberPhase,
                        MarkTempObjectPhase,
                        MarkTempNumberOnTempObjectPhase,
                    SpeculationPropagationAnalysisPhase,
            DumpGlobOptInstrPhase, // Print the Globopt instr string in post lower dumps
            LowererPhase,
                FastPathPhase,
                    LoopFastPathPhase,
                    MathFastPathPhase,
                    AtomPhase,
                        MulStrengthReductionPhase,
                        AgenPeepsPhase,
                    BranchFastPathPhase,
                    CallFastPathPhase,
                    BitopsFastPathPhase,
                    OtherFastPathPhase,
                    ObjectFastPathPhase,
                    ProfileBasedFldFastPathPhase,
                    AddFldFastPathPhase,
                    RootObjectFldFastPathPhase,
                    ArrayLiteralFastPathPhase,
                    ArrayCtorFastPathPhase,
                    NewScopeSlotFastPathPhase,
                    FrameDisplayFastPathPhase,
                    HoistMarkTempInitPhase,
                    HoistConstAddrPhase,
                JitWriteBarrierPhase,
                PreLowererPeepsPhase,
                CFGInJitPhase,
                TypedArrayPhase,
                TracePinnedTypesPhase,
            InterruptProbePhase,
            EncodeConstantsPhase,
            RegAllocPhase,
                LivenessPhase,
                    RegParamsPhase,
                LinearScanPhase,
                    OpHelperRegOptPhase,
                    StackPackPhase,
                    SecondChancePhase,
                    RegionUseCountPhase,
                    RegHoistLoadsPhase,
                    ClearRegLoopExitPhase,
            PeepsPhase,
            LayoutPhase,
            EHBailoutPatchUpPhase,
            FinalLowerPhase,
            PrologEpilogPhase,
            InsertNOPsPhase,
            EncoderPhase,
                AssemblyPhase,
                EmitterPhase,
                DebugBreakPhase,
    #if defined(_M_X64)
                BrShortenPhase,
                    LoopAlignPhase,
    #endif

    #ifdef RECYCLER_WRITE_BARRIER
    #if DBG_DUMP
        SWBPhase,
    #endif
    #endif
        RunPhase,
            InterpreterPhase,
            EvalCompilePhase,
                FastIndirectEvalPhase,
            IdleDecommitPhase,
            IdleCollectPhase,
            MarshalPhase,
            MemoryAllocationPhase,
    #ifdef RECYCLER_PAGE_HEAP
                PageHeapPhase,
    #endif
                LargeMemoryAllocationPhase,
                PageAllocatorAllocPhase,
            RecyclerPhase,
                ThreadCollectPhase,
                ExplicitFreePhase,
                ExpirableCollectPhase,
                GarbageCollectPhase,
                ConcurrentCollectPhase,
                    BackgroundResetMarksPhase,
                    BackgroundFindRootsPhase,
                    BackgroundRescanPhase,
                    BackgroundRepeatMarkPhase,
                    BackgroundFinishMarkPhase,
                ConcurrentPartialCollectPhase,
                ParallelMarkPhase,
                PartialCollectPhase,
                    ResetMarksPhase,
                    ResetWriteWatchPhase,
                    FindRootPhase,
                        FindRootArenaPhase,
                        FindImplicitRootPhase,
                        FindRootExtPhase,
                    ScanStackPhase,
                    ConcurrentMarkPhase,
                    ConcurrentWaitPhase,
                    RescanPhase,
                    MarkPhase,
                    SweepPhase,
                        SweepWeakPhase,
                        SweepSmallPhase,
                        SweepLargePhase,
                        SweepPartialReusePhase,
                    ConcurrentSweepPhase,
                    FinalizePhase,
                    DisposePhase,
                    FinishPartialPhase,
            HostPhase,
            BailOutPhase,
            BailInPhase,
            GeneratorGlobOptPhase,
            RegexQcPhase,
            RegexOptBTPhase,
            InlineCachePhase,
            PolymorphicInlineCachePhase,
            MissingPropertyCachePhase,
            PropertyCachePhase, // Trace caching of property lookups using PropertyString and JavascriptSymbol
            CloneCacheInCollisionPhase,
            ConstructorCachePhase,
            InlineCandidatePhase,
            ScriptFunctionWithInlineCachePhase,
            IsConcatSpreadableCachePhase,
            ArenaPhase,
            ApplyUsagePhase,
            ObjectHeaderInliningPhase,
                ObjectHeaderInliningForConstructorsPhase,
                ObjectHeaderInliningForObjectLiteralsPhase,
                ObjectHeaderInliningForEmptyObjectsPhase,
            OptUnknownElementNamePhase,
            TypePropertyCachePhase,
    #if DBG_DUMP
            InlineSlotsPhase,
    #endif
            DynamicProfilePhase,
    #ifdef DYNAMIC_PROFILE_STORAGE
            DynamicProfileStoragePhase,
    #endif
            JITLoopBodyPhase,
            JITLoopBodyInTryCatchPhase,
            JITLoopBodyInTryFinallyPhase,
            ReJITPhase,
            ExecutionModePhase,
            SimpleJitDynamicProfilePhase,
            SimpleJitPhase,
            FullJitPhase,
            FailNativeCodeInstallPhase,
            PixelArrayPhase,
            EtwPhase,
            ProfilerPhase,
            CustomHeapPhase,
            XDataAllocatorPhase,
            PageAllocatorPhase,
            StringConcatPhase,
    #if DBG_DUMP
            PRNGPhase,
    #endif
            PreReservedHeapAllocPhase,
            CFGPhase,
            ExceptionStackTracePhase,
            ExtendedExceptionInfoStackTracePhase,
            TypeHandlerTransitionPhase,
            DebuggerPhase,
                ENCPhase,
            ConsoleScopePhase,
            ScriptProfilerPhase,
            JSONPhase,
            IntlPhase,
            RegexResultNotUsedPhase,
            ErrorPhase,
            PropertyRecordPhase,
            TypePathDynamicSizePhase,
            ObjectCopyPhase,
            ShareTypesWithAttributesPhase,
            ShareAccessorTypesPhase,
            ShareFuncTypesPhase,
            ShareCrossSiteFuncTypesPhase,
            ConditionalCompilationPhase,
            InterpreterProfilePhase,
            InterpreterAutoProfilePhase,
            ByteCodeConcatExprOptPhase,
            TraceInlineCacheInvalidationPhase,
            TracePropertyGuardsPhase,
            PerfHintPhase,
            TypeShareForChangePrototypePhase,
            DeferSourceLoadPhase,
            DataCachePhase,
            ObjectMutationBreakpointPhase,
            NativeCodeDataPhase,
            XDataPhase,
        PhaseCount,
        InvalidPhase
    };

    extern const char16_t* const FlagNames[FlagCount + 1];
    extern const char16_t* const PhaseNames[PhaseCount + 1];
    extern const Flag           FlagParents[FlagCount + 1];

    typedef     int             Number;
    typedef     bool            Boolean;

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class String
    ///
    /// A simple string wrapper for storing strings for the flags. It creates a
    /// copy of the string which it releases in the destructor. Simple operator
    /// fiddling to make this class as interoperable with strings as possible :-
    ///
    ///     1.  Single argument constructor takes care of initialization
    ///     2.  Assignment operator overloaded for char16_t*
    ///     3.  JavascriptConversion to const char16_t * is defined. Making this constant ensures
    ///         that a non constant char16_t * does not point to our buffer which can
    ///         potentially corrupt it.
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    class String
    {
        PREVENT_COPYCONSTRUCT(String)


    // Data
    private:
        char16_t*           pszValue;

    // Construction
    public:
        inline String();
        inline String(__in_z_opt const char16_t* psz);
        inline ~String();


    // Methods
    public:

        ///----------------------------------------------------------------------------
        ///
        /// Assignment operator defined for easy access
        ///
        ///----------------------------------------------------------------------------

        String& operator=(__in_z_opt const char16_t* psz)
        {
            Set(psz);
            return *this;
        }

        String &operator =(const String &other)
        {
            return (*this = other.pszValue);
        }

        ///----------------------------------------------------------------------------
        ///
        /// Typecast operator defined so we can directly use it with strings
        ///
        ///----------------------------------------------------------------------------

        operator const char16_t* () const
        {
            return this->pszValue;
        }

    // Implementation
    private:
        void Set(__in_z_opt const char16_t* pszValue);
    };

    class NumberSet
    {
    public:
        NumberSet();
        void Add(uint32_t x);
        bool Contains(uint32_t x);
        bool Empty() const { return set.Count() == 0; }
    private:
        JsUtil::BaseHashSet<uint32_t, NoCheckHeapAllocator, PrimeSizePolicy> set;
    };

    class NumberPair
    {
    public:
        NumberPair(uint32_t x, uint32_t y) : x(x), y(y) {}
        NumberPair() : x((uint32_t)-1), y((uint32_t)-1) {}

        operator hash_t() const { return (x << 16) + y; }
        bool operator ==(const NumberPair &other) const { return x == other.x && y == other.y; }
    private:
        uint32_t x;
        uint32_t y;
    };

    class NumberPairSet
    {
    public:
        NumberPairSet();
        void Add(uint32_t x, uint32_t y);
        bool Contains(uint32_t x, uint32_t y);
        bool Empty() const { return set.Count() == 0; }
    private:
        JsUtil::BaseHashSet<NumberPair, NoCheckHeapAllocator, PrimeSizePolicy> set;
    };

    class NumberTrio
    {
    public:
        NumberTrio(uint32_t x, uint32_t y, uint32_t z) : x(x), y(y), z(z) {}
        NumberTrio() : x((uint32_t)-1), y((uint32_t)-1) {}

        operator hash_t() const { return (x << 20) + (y << 10) + z; }
        bool operator ==(const NumberTrio &other) const { return x == other.x && y == other.y && z == other.z; }
    private:
        uint32_t x;
        uint32_t y;
        uint32_t z;
    };

    class NumberTrioSet
    {
    public:
        NumberTrioSet();
        void Add(uint32_t x, uint32_t y, uint32_t z);
        bool Contains(uint32_t x, uint32_t y, uint32_t z);
        bool Empty() const { return set.Count() == 0; }
    private:
        JsUtil::BaseHashSet<NumberTrio, NoCheckHeapAllocator, PrimeSizePolicy> set;
    };

    struct SourceFunctionNode
    {
        uint sourceContextId;
        Js::LocalFunctionId functionId;
        SourceFunctionNode(uint sourceContextId, Js::LocalFunctionId functionId) : sourceContextId(sourceContextId), functionId(functionId) {}
        SourceFunctionNode() : sourceContextId(0), functionId(0){}
    };

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// struct RangeUnit
    ///
    /// Used to store a pair of bounds
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------

    template <typename TRangeUnitData>
    struct RangeUnit
    {
        TRangeUnitData i, j;
        RangeUnit() {}
        RangeUnit(TRangeUnitData x, TRangeUnitData y) :
            i(x),
            j(y)
        {}
    };

    template <typename TRangeUnitData>
    bool RangeUnitContains(RangeUnit<TRangeUnitData> unit, TRangeUnitData item)
    {
        return (item >= unit.i && item <= unit.j);
    }

    template <>
    bool RangeUnitContains<SourceFunctionNode>(RangeUnit<SourceFunctionNode> unit, SourceFunctionNode n);

    template <typename TRangeUnitData>
    RangeUnit<TRangeUnitData> GetFullRange()
    {
        RangeUnit<TRangeUnitData> unit;
        unit.i = INT_MIN;
        unit.j = INT_MAX;
        return unit;
    }

    template <>
    RangeUnit<SourceFunctionNode> GetFullRange();

    template <typename TRangeUnitData>
    TRangeUnitData GetPrevious(TRangeUnitData unit)
    {
        return unit - 1;
    }

    template <>
    SourceFunctionNode GetPrevious(SourceFunctionNode unit);

    template <typename TRangeUnitData>
    TRangeUnitData GetNext(TRangeUnitData unit)
    {
        return unit + 1;
    }

    template <>
    SourceFunctionNode GetNext(SourceFunctionNode unit);

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class Range
    ///
    /// Data structure to store a range of integers. (Can trivially be templatized
    /// but resisting the temptation for the sake of simplicity.)
    ///
    /// Sample:     1,3-11,15,20
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------

    template <typename TRangeUnitData>
    class RangeBase
    {
    // Nested Classes
    public:
        typedef RangeUnit<TRangeUnitData> Unit;


    // Data
    protected:
        SList<Unit, NoCheckHeapAllocator>      range;

    // Construction
    public:
        RangeBase() : range(&NoCheckHeapAllocator::Instance) {}

    // Methods
    public:
        inline  bool            InRange(TRangeUnitData i);
        inline  bool            ContainsAll();
        inline  void            Add(TRangeUnitData i, RangeBase<TRangeUnitData>* oppositeRange = nullptr);
        inline  void            Add(TRangeUnitData i, TRangeUnitData j, RangeBase<TRangeUnitData>* oppositeRange = nullptr);
        inline  void            Clear();

#if DBG_RANGE
        template <typename TFunction>
        void Map(TFunction fn) const
        {
            range.Map(fn);
        }
#endif
    };

    class Range : public RangeBase<SourceFunctionNode> {};
    class NumberRange : public RangeBase<int> {};

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class Phases
    ///
    /// Maintains the list of all the phases. Each individual phase can be enabled
    /// and an optional range of numbers can be added to it
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    class Phases
    {
    // Nested Types
    private:

        ///----------------------------------------------------------------------------
        ///----------------------------------------------------------------------------
        ///
        /// struct UnitPhase
        ///
        /// Stores information about a particular phase. Current information is :
        ///     1.  Is the phase enabled/disabled?
        ///     2.  A range of numbers for the given phase. This range can represent a
        ///         variety of information. For example, this range can be the function
        ///         numbers of all the functions where a particular optimization is
        ///         enabled or where opcode dump is enabled.
        ///
        ///
        ///  Its a tiny private struct used by Phases for simply storing data. So
        ///  making this a struct with all members public.
        ///
        ///----------------------------------------------------------------------------
        ///----------------------------------------------------------------------------

        struct UnitPhase
        {

        // Construction
        public:
            UnitPhase() : valid(false) { }

        // Data
        public:
                bool            valid;
                Range           range;

        };

    // Data
    private:
                UnitPhase       phaseList[PhaseCount];

    // Constructors
    public:
        Phases() {}

    // Methods
    public:

        void            Enable(Phase phase);
        void            Disable(Phase phase);
        bool            IsEnabled(Phase phase);
        bool            IsEnabled(Phase phase, uint sourceContextId, Js::LocalFunctionId functionId);
        bool            IsEnabledForAll(Phase phase);
        Range *         GetRange(Phase phase);
        Phase           GetFirstPhase();
    };

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class ConfigFlagsTable
    ///
    /// Maintains the list of all the flags. Individual flags can be accessed by :-
    ///
    ///     1.  flagTable->verbose;     // here verbose is the name of the flag
    ///                                 // It will be strongly typed with the
    ///                                 // type of the flag is as described in
    ///                                 // ConfigFlagsTable.h
    ///
    ///     2.  flagTable->GetAsBoolean(verboseFlag);
    ///                                 // this access is weekly typed. Even if the
    ///                                 // flag is not of type Boolean, it will be
    ///                                 // reinterpret_cast to Boolean *
    ///
    /// The former access should be used in the code for reading the flag values
    /// while the later is useful for adding values into the table.
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    class ConfigFlagsTable
    {

    // Constructor
    public:
        ConfigFlagsTable();

    // Methods
    public:

        static  Flag            GetFlag(const char16_t* str);
        static  Phase           GetPhase(const char16_t* str);
        static  void            PrintUsageString();

        static  FlagTypes       GetFlagType(Flag flag);

                String*         GetAsString(Flag flag) const;
                Phases*         GetAsPhase(Flag flag) const;
                Flag            GetOppositePhaseFlag(Flag flag) const;
                Boolean*        GetAsBoolean(Flag flag) const;
                Number*         GetAsNumber(Flag flag) const;
                NumberSet*      GetAsNumberSet(Flag flag) const;
                NumberPairSet * GetAsNumberPairSet(Flag flag) const;
                NumberTrioSet * GetAsNumberTrioSet(Flag flag) const;
                NumberRange *   GetAsNumberRange(Flag flag) const;

                void            SetAsBoolean(Flag flag, Boolean value);

                Boolean         GetDefaultValueAsBoolean(Flag flag) const;

        // indicates whether a flag is a parent flag
                bool            IsParentFlag(Flag flag) const;

        // get the parent flag of a given flag, if any, otherwise returns InvalidFlag.
                Flag            GetParentFlag(Flag flag) const;

        // get the next child flag of a given parent flag, if any, otherwise returns InvalidFlag. Pass InvalidFlag as currentChildFlag if no current child flag to iterate from.
                Flag            GetNextChildFlag(Flag parentFlag, Flag currentChildFlag) const;

                void            Enable(Flag flag);
                bool            IsEnabled(Flag flag);
                void            Disable(Flag flag);

                void             VerboseDump();

    // Data
    public:

        ///----------------------------------------------------------------------------
        ///
        /// Declaration of each flag as a member variable of the corresponding type.
        /// These variables are made public because get/set are too ugly for simple
        /// flags. Besides there are flags like phase lists which are not simple and
        /// its better to have a uniform way to access them.
        ///
        ///     if(ConfigFlag->dump)     is much better than
        ///     if(ConfigFlag->GetDump())
        ///
        ///----------------------------------------------------------------------------

        // TODO (hanhossain): ConfigFlagsList start
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

        #ifdef ENABLE_SPECTRE_RUNTIME_MITIGATIONS
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
        #else
        #define DEFAULT_CONFIG_MitigateSpectre (false)

        #define DEFAULT_CONFIG_AddMaskingBlocks (false)

        #define DEFAULT_CONFIG_PoisonVarArrayLoad (false)
        #define DEFAULT_CONFIG_PoisonIntArrayLoad (false)
        #define DEFAULT_CONFIG_PoisonFloatArrayLoad (false)
        #define DEFAULT_CONFIG_PoisonTypedArrayLoad (false)
        #define DEFAULT_CONFIG_PoisonStringLoad (false)
        #define DEFAULT_CONFIG_PoisonObjectsForLoads (false)

        #define DEFAULT_CONFIG_PoisonVarArrayStore (false)
        #define DEFAULT_CONFIG_PoisonIntArrayStore (false)
        #define DEFAULT_CONFIG_PoisonFloatArrayStore (false)
        #define DEFAULT_CONFIG_PoisonTypedArrayStore (false)
        #define DEFAULT_CONFIG_PoisonStringStore (false)
        #define DEFAULT_CONFIG_PoisonObjectsForStores (false)
        #endif

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

        #if DBG
            Boolean ArrayValidate;
            Boolean MemOpMissingValueValidate;
            Boolean OOPJITFixupValidate;
        #endif
        #ifdef ARENA_MEMORY_VERIFY
            Boolean ArenaNoFreeList;
            Boolean ArenaNoPageReuse;
            Boolean ArenaUseHeapAlloc;
        #endif
        Boolean ValidateInlineStack;
        Boolean AsmDiff;
        String AsmDumpMode;
        Boolean AsmJs;
        Boolean AsmJsStopOnError;
        Boolean AsmJsEdge;
        Boolean Wasm;
        Boolean WasmI64;
        Boolean WasmFastArray;
        Boolean WasmSharedArrayVirtualBuffer;
        Boolean WasmMathExFilter;
        Boolean WasmCheckVersion;
        Boolean WasmAssignModuleID;
        Boolean WasmIgnoreLimits;
        Boolean WasmFold;
        Boolean WasmIgnoreResponse;
        Number WasmMaxTableSize;
        Boolean WasmThreads;
        Boolean WasmMultiValue;
        Boolean WasmSignExtends;
        Boolean WasmNontrapping;

        // WebAssembly Experimental Features
        // Master WasmExperimental flag to activate WebAssembly experimental features
        Boolean WasmExperimental;

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        Boolean WasmSimd;

        Boolean AssertBreak;
        Boolean AssertPopUp;
        Boolean AssertIgnore;
        Boolean AsyncDebugging;
        Number BailOnNoProfileLimit;
        Number BailOnNoProfileRejitLimit;
        Boolean BaselineMode;
        String DumpOnCrash;
        String FullMemoryDump;
        #ifdef BAILOUT_INJECTION
            NumberPairSet BailOut;
            Boolean BailOutAtEveryLine;
            Boolean BailOutAtEveryByteCode;
            Boolean BailOutAtEveryImplicitCall;
            NumberSet BailOutByteCode;
        #endif
        Boolean Benchmark;
        Boolean BgJit;
        Boolean BgParse;
        Number BgJitDelay;
        Number BgJitDelayFgBuffer;
        Number BgJitPendingFuncCap;

        Boolean CreateFunctionProxy;
        Boolean HybridFgJit;
        Number HybridFgJitBgQueueLengthThreshold;
        Boolean BytecodeHist;
        Boolean CurrentSourceInfo;
        Boolean CFGLog;
        Boolean CheckAlignment;
        Boolean CheckEmitBufferPermissions;
        #ifdef CHECK_MEMORY_LEAK
            Boolean CheckMemoryLeak;
            String DumpOnLeak;
        #endif
        Boolean CheckOpHelpers;
        Boolean CloneInlinedPolymorphicCaches;
        Boolean ConcurrentRuntime;
        Number ConstructorInlineThreshold;
        Number ConstructorCallsRequiredToFinalizeCachedType;
        Number PropertyCacheMissPenalty;
        Number PropertyCacheMissThreshold;
        Number PropertyCacheMissReset;
        Boolean Debug;
        NumberSet DebugBreak;
        NumberTrioSet StatementDebugBreak;
        Phases DebugBreakOnPhaseBegin;

        Boolean DebugWindow;
        Boolean ParserStateCache;
        Boolean CompressParserStateCache;
        Boolean DeferTopLevelTillFirstCall;
        Number DeferParse;
        Boolean DirectCallTelemetryStats;
        Boolean DisableArrayBTree;
        Boolean DisableRentalThreading;
        Boolean DisableVTuneSourceLineInfo;
        Boolean DisplayMemStats;
        Phases Dump;
        #ifdef DUMP_FRAGMENTATION_STATS
            Boolean DumpFragmentationStats;
        #endif
        Boolean DumpIRAddresses;
        Boolean DumpLineNoInColor;
        #ifdef RECYCLER_DUMP_OBJECT_GRAPH
            Boolean DumpObjectGraphOnExit;
            Boolean DumpObjectGraphOnCollect;
        #endif
        Boolean DumpEvalStringOnRemoval;
        Boolean DumpObjectGraphOnEnum;
        #ifdef DYNAMIC_PROFILE_STORAGE
            String DynamicProfileCache; String Dpc;
            String DynamicProfileCacheDir;
            String DynamicProfileInput; String Dpi;
        #endif
        #ifdef EDIT_AND_CONTINUE
            Boolean EditTest;
        #endif
        Boolean WininetProfileCache;
        Boolean NoDynamicProfileInMemoryCache;
        Boolean ProfileBasedSpeculativeJit;
        Number ProfileBasedSpeculationCap;
        Boolean ExecuteByteCodeBufferReturnsInvalidByteCode;
        Number ExpirableCollectionGCCount;
        Number ExpirableCollectionTriggerThreshold;
        Boolean SkipSplitOnNoResult;
        Boolean Force32BitByteCode;

        Boolean CollectGarbage;

        Boolean Intl;
        Boolean IntlBuiltIns;
        Boolean IntlPlatform;

        Boolean JsBuiltIn;
        Boolean JitRepro;
        Boolean EntryPointInfoRpcData;

        Boolean LdChakraLib;
        Boolean TestChakraLib;

        // ES6 (BLUE+1) features/flags

        // Master ES6 flag to enable STABLE ES6 features/flags
        Boolean ES6;

        // Master ES6 flag to enable ALL sub ES6 features/flags
        Boolean ES6All;

        // Master ES6 flag to enable Threshold ES6 features/flags
        Boolean ES6Experimental;

        // Per ES6 feature/flag

        Boolean ES7AsyncAwait;
        Boolean ES6DateParseFix;
        Boolean ES6FunctionNameFull;
        Boolean ES6Generators;
        Boolean ES7ExponentiationOperator;

        Boolean ES7ValuesEntries;
        Boolean ES7TrailingComma;
        Boolean ES6IsConcatSpreadable;
        Boolean ES6Math;

        #ifndef COMPILE_DISABLE_ESDynamicImport
        #define COMPILE_DISABLE_ESDynamicImport 0
        #endif
        Boolean ESDynamicImport;

        Boolean ES6Module;
        Boolean ES6Object;
        Boolean ES6Number;
        Boolean ES6ObjectLiterals;
        Boolean ES6Proxy;
        Boolean ES6Rest;
        Boolean ES6Spread;
        Boolean ES6String;
        Boolean ES6StringPrototypeFixes;
        Boolean ES2018ObjectRestSpread;

        Boolean ES6PrototypeChain;
        Boolean ES6ToPrimitive;
        Boolean ES6ToLength;
        Boolean ES6ToStringTag;
        Boolean ES6Unicode;
        Boolean ES6UnicodeVerbose;
        Boolean ES6Unscopables;
        Boolean ES6RegExSticky;
        Boolean ES2018RegExDotAll;
        Boolean ESExportNsAs;
        Boolean ES2018AsyncIteration;
        Boolean ESTopLevelAwait;

        #ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
            #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
        #endif
        Boolean ES6RegExPrototypeProperties;

        #ifndef COMPILE_DISABLE_ES6RegExSymbols
            #define COMPILE_DISABLE_ES6RegExSymbols 0
        #endif

        // When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
        // sets implicit call flag before calling into script
        // Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        Boolean ES6RegExSymbols;

        Boolean ES6Verbose;
        Boolean ESObjectGetOwnPropertyDescriptors;

        #ifndef COMPILE_DISABLE_ESSharedArrayBuffer
            #define COMPILE_DISABLE_ESSharedArrayBuffer 0
        #endif

        Boolean ESSharedArrayBuffer;

        // Newer language feature flags

        // ES BigInt flag
        Boolean ESBigInt;

        // ES Numeric Separator support for numeric constants
        Boolean ESNumericSeparator;

        // ES Nullish coalescing operator support (??)
        Boolean ESNullishCoalescingOperator;

        // ES Hashbang support for interpreter directive syntax
        Boolean ESHashbang;

        // ES Symbol.prototype.description flag
        Boolean ESSymbolDescription;

        Boolean ESArrayFindFromLast;

        // ES Promise.any and AggregateError flag
        Boolean ESPromiseAny;

        // ES import.meta keyword meta-property
        Boolean ESImportMeta;

        //ES globalThis flag
        Boolean ESGlobalThis;

        // This flag to be removed once JITing generator functions is stable
        Boolean JitES6Generators;

        Boolean FastLineColumnCalculation;
        String Filename;
        Boolean FreeRejittedCode;
        Boolean ForceGuardPages;
        Boolean PrintGuardPageBounds;
        Boolean ForceLegacyEngine;
        Phases Force;
        Phases Stress;
        Boolean ForceArrayBTree;
        Boolean StrongArraySort;
        Boolean ForceCleanPropertyOnCollect;
        Boolean ForceCleanCacheOnCollect;
        Boolean ForceGCAfterJSONParse;
        Boolean ForceDecommitOnCollect;
        Boolean ForceDeferParse;
        Boolean ForceDiagnosticsMode;
        Boolean ForceGetWriteWatchOOM;
        Boolean ForcePostLowerGlobOptInstrString;
        Boolean ForceSplitScope;
        Boolean EnumerateSpecialPropertiesInDebugger;
        Boolean EnableContinueAfterExceptionWrappersForHelpers;
        Boolean EnableContinueAfterExceptionWrappersForBuiltIns;
        Boolean EnableFunctionSourceReportForHeapEnum;
        Number ForceFragmentAddressSpace;
        Number ForceOOMOnEBCommit;
        Boolean ForceDynamicProfile;
        Boolean ForceES5Array;
        Boolean ForceAsmJsLinkFail;
        Boolean ForceExpireOnNonCacheCollect;
        Boolean ForceFastPath;
        Boolean ForceFloatPref;
        Boolean ForceJITLoopBody;
        Boolean ForceStaticInterpreterThunk;
        Boolean DumpCommentsFromReferencedFiles;
        Number DelayFullJITSmallFunc;
        Boolean EnableFatalErrorOnOOM;

        #if defined(_M_ARM32_OR_ARM64)
            Boolean ForceLocalsPtr;
        #endif
        Boolean DeferLoadingAvailableSource;
        Boolean ForceNative;
        Boolean ForceSerialized;
        Number ForceSerializedBytecodeMajorVersion;
        Number ForceSerializedBytecodeVersionSchema;
        Boolean ForceStrictMode;
        Boolean ForceUndoDefer;
        Boolean ForceBlockingConcurrentCollect;
        Boolean FreTestDiagMode;
        #ifdef BYTECODE_TESTING
            Number ByteCodeBranchLimit;
            Boolean MediumByteCodeLayout;
            Boolean LargeByteCodeLayout;
        #endif
        Number InduceCodeGenFailure;
        Number InduceCodeGenFailureSeed;
        Number InjectPartiallyInitializedInterpreterFrameError;
        Number InjectPartiallyInitializedInterpreterFrameErrorType;
        Boolean GenerateByteCodeBufferReturnsCantGenerate;
        Number GoptCleanupThreshold;
        Number AsmGoptCleanupThreshold;
        Boolean HighPrecisionDate;
        Number InlineCountMax;
        Number InlineCountMaxInLoopBodies; Number icminlb;
        Number InlineInLoopBodyScaleDownFactor; Number iilbsdf;
        Number InlineThreshold;
        Number AggressiveInlineCountMax;
        Number AggressiveInlineThreshold;
        Number InlineThresholdAdjustCountInLargeFunction;
        Number InlineThresholdAdjustCountInMediumSizedFunction;
        Number InlineThresholdAdjustCountInSmallFunction;
        Number AsmJsInlineAdjust;
        String Interpret;
        Phases Instrument;
        Number JitQueueThreshold;
        #ifdef LEAK_REPORT
            String LeakReport;
        #endif
        Number LoopInlineThreshold;
        Number LeafInlineThreshold;
        Number ConstantArgumentInlineThreshold;
        Number RecursiveInlineThreshold;
        Number RecursiveInlineDepthMax;
        Number RecursiveInlineDepthMin;
        Number RedeferralCap;
        Number Loop;
        Number LoopInterpretCount; Number lic;
        Number LoopProfileIterations;
        Number OutsideLoopInlineThreshold;
        Number MaxFuncInlineDepth;
        Number MaxNumberOfInlineesWithLoop;
        #ifdef MEMSPECT_TRACKING
            Phases Memspect;
        #endif
        Number PolymorphicInlineThreshold;
        Boolean PrimeRecycler;
        Boolean TraceEngineRefcount;
        #if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
            Boolean LeakStackTrace;
            Boolean ForceMemoryLeak;
        #endif
        Boolean DumpAfterFinalGC;
        Boolean ForceOldDateAPI;

        Number JitLoopBodyHotLoopThreshold;
        Number LoopBodySizeThresholdToDisableOpts;

        Number MaxJitThreadCount;
        Boolean ForceMaxJitThreadCount;

        Boolean MitigateSpectre;

        Boolean AddMaskingBlocks;

        Boolean PoisonVarArrayLoad;
        Boolean PoisonIntArrayLoad;
        Boolean PoisonFloatArrayLoad;
        Boolean PoisonTypedArrayLoad;
        Boolean PoisonStringLoad;
        Boolean PoisonObjectsForLoads;

        Boolean PoisonVarArrayStore;
        Boolean PoisonIntArrayStore;
        Boolean PoisonFloatArrayStore;
        Boolean PoisonTypedArrayStore;
        Boolean PoisonStringStore;
        Boolean PoisonObjectsForStores;

        Number MinInterpretCount;
        Number MinSimpleJitRunCount;
        Number MaxInterpretCount; Number Mic;
        Number MaxSimpleJitRunCount; Number Msjrc;
        Number MinMemOpCount; Number Mmoc;

        #if ENABLE_COPYONACCESS_ARRAY
            Number MaxCopyOnAccessArrayLength;
            Number MinCopyOnAccessArrayLength;
            Number CopyOnAccessArraySegmentCacheSize;
        #endif

        Number MinTemplatizedJitRunCount;
        Number MinAsmJsInterpreterRunCount;

        Number MinTemplatizedJitLoopRunCount;
        Number MaxTemplatizedJitRunCount; Number Mtjrc;
        Number MaxAsmJsInterpreterRunCount; Number Maic;

        Number AutoProfilingInterpreter0Limit;
        Number ProfilingInterpreter0Limit;
        Number AutoProfilingInterpreter1Limit;
        Number SimpleJitLimit;
        Number ProfilingInterpreter1Limit;

        String ExecutionModeLimits; String Eml;
        Boolean EnforceExecutionModeLimits; Boolean Eeml;

        Number SimpleJitAfter; Number Sja;
        Number FullJitAfter; Number Fja;

        Boolean NewSimpleJit;

        Number MaxLinearIntCaseCount;
        Number MaxSingleCharStrJumpTableSize;
        Number MaxSingleCharStrJumpTableRatio;
        Number MinSwitchJumpTableSize;
        Number MaxLinearStringCaseCount;
        Number MinDeferredFuncTokenCount;
        #if DBG
            Number SkipFuncCountForBailOnNoProfile;
        #endif
        Number MaxJITFunctionBytecodeByteLength;
        Number MaxJITFunctionBytecodeCount;
        Number MaxLoopsPerFunction;
        Number FuncObjectInlineCacheThreshold;
        Boolean NoDeferParse;
        Boolean NoLogo;
        Boolean OOPJITMissingOpts;
        Boolean CrashOnOOPJITFailure;
        Boolean OOPCFGRegistration;
        Boolean ForceJITCFGCheck;
        Boolean UseJITTrampoline;
        Boolean NoNative;
        Number NopFrequency;
        Boolean NoStrictMode;
        Boolean NormalizeStats;
        Phases Off;
        Phases OffProfiledByteCode;
        Phases On;
        String OutputFile;
        String OutputFileOpenMode;
        #ifdef ENABLE_TRACE
            Boolean InMemoryTrace;
            Number InMemoryTraceBufferSize;
            #ifdef STACK_BACK_TRACE
                Boolean TraceWithStack;
            #endif // STACK_BACK_TRACE
        #endif // ENABLE_TRACE
        Boolean PrintRunTimeDataCollectionTrace;
        #ifdef ENABLE_PREJIT
            Boolean Prejit;
        #endif
        Boolean PrintSrcInDump;
        #if PROFILE_DICTIONARY
            Number ProfileDictionary;
        #endif
        #ifdef PROFILE_EXEC
            Phases Profile;
            Number ProfileThreshold;
        #endif
        #ifdef PROFILE_OBJECT_LITERALS
            Boolean ProfileObjectLiteral;
        #endif
        #ifdef PROFILE_MEM
            String ProfileMemory;
        #endif
        #ifdef PROFILE_STRINGS
            Boolean ProfileStrings;
        #endif
        #ifdef PROFILE_TYPES
            Boolean ProfileTypes;
        #endif
        #ifdef PROFILE_EVALMAP
            Boolean ProfileEvalMap;
        #endif

        #ifdef PROFILE_BAILOUT_RECORD_MEMORY
            Boolean ProfileBailOutRecordMemory;
        #endif

        #if DBG
            Boolean ValidateIntRanges;
        #endif
        Number RejitMaxBailOutCount;
        Number CallsToBailoutsRatioForRejit;
        Number LoopIterationsToBailoutsRatioForRejit;
        Number MinBailOutsBeforeRejit;
        Number MinBailOutsBeforeRejitForLoops;
        Boolean LibraryStackFrame;
        Boolean LibraryStackFrameDebugger;
        #ifdef RECYCLER_STRESS
            Boolean RecyclerStress;
            #if ENABLE_CONCURRENT_GC
                Boolean RecyclerBackgroundStress;
                Boolean RecyclerConcurrentStress;
                Boolean RecyclerConcurrentRepeatStress;
            #endif
            #if ENABLE_PARTIAL_GC
                Boolean RecyclerPartialStress;
            #endif
            Boolean RecyclerTrackStress;
            Boolean RecyclerInduceFalsePositives;
        #endif // RECYCLER_STRESS
        Boolean RecyclerForceMarkInterior;
        #if ENABLE_CONCURRENT_GC
            Number RecyclerPriorityBoostTimeout;
            Number RecyclerThreadCollectTimeout;
            Boolean EnableConcurrentSweepAlloc; Boolean ecsa;
        #endif
        #ifdef RECYCLER_PAGE_HEAP
            Number PageHeap;
            Boolean PageHeapAllocStack;
            Boolean PageHeapFreeStack;
            NumberRange PageHeapBucketNumber;
            Number PageHeapBlockType;
            Boolean PageHeapDecommitGuardPage;
        #endif
        #ifdef RECYCLER_NO_PAGE_REUSE
            Boolean RecyclerNoPageReuse;
        #endif
        #ifdef RECYCLER_MEMORY_VERIFY
            Phases RecyclerVerify;
            Number RecyclerVerifyPadSize;
        #endif
        Boolean RecyclerTest;
        Boolean RecyclerProtectPagesOnRescan;
        #ifdef RECYCLER_VERIFY_MARK
            Boolean RecyclerVerifyMark;
        #endif
        Number LowMemoryCap;
        Number NewPagesCapDuringBGSweeping;
        Number AllocPolicyLimit;
        #ifdef RUNTIME_DATA_COLLECTION
            String RuntimeDataOutputFile;
        #endif
        Number SpeculationCap;
        #if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
            Phases Stats;
        #endif
        #if EXCEPTION_RECOVERY
            Boolean SwallowExceptions;
        #endif
        Boolean PrintSystemException;
        Number SwitchOptHolesThreshold;
        Number TempMin;
        Number TempMax;
        Phases Trace;

        #if defined(_M_X64)
            Number LoopAlignNopLimit;
        #endif

        #ifdef PROFILE_MEM
            Phases TraceMemory;
        #endif
        #if DBG_DUMP || defined(RECYCLER_TRACE)
            //TraceMetaDataParsing flag with optional levels:
            //    Level 1 = interfaces only
            //    Level 2 = interfaces and methods
            //    Level 3 = interfaces, methods and parameters
            //    Level 4 = interfaces and properties
            //    Level 5 (default) = ALL
            Number TraceMetaDataParsing;
            Boolean TraceWin8Allocations;
            Boolean TraceWin8DeallocationsImmediate;
            Boolean PrintWin8StatsDetailed;
            Boolean TraceProtectPages;
        #endif
        Boolean TraceAsyncDebugCalls;
        #ifdef TRACK_DISPATCH
            Boolean TrackDispatch;
        #endif
        Boolean Verbose;
        Boolean UseFullName;
        Boolean Utf8;
        Number Version;
        Boolean WERExceptionSupport;
        Boolean ExtendedErrorStackForTestHost;
        Boolean errorStackTrace;
        Boolean DoHeapEnumOnEngineShutdown;
        #ifdef HEAP_ENUMERATION_VALIDATION
            Boolean ValidateHeapEnum;
        #endif

        #if ENABLE_REGEX_CONFIG_OPTIONS
            //
            // Regex flags
            //
            Boolean RegexTracing;
            Boolean RegexProfile;
            Boolean RegexDebug;
            Boolean RegexDebugAST;
            Boolean RegexDebugAnnotatedAST;
            Boolean RegexBytecodeDebug;
            Boolean RegexOptimize;
            Number DynamicRegexMruListSize;
        #endif

        Boolean OptimizeForManyInstances;
        Boolean EnableArrayTypeMutation;
        Number ArrayMutationTestSeed;
        Phases TestTrace;
        Boolean EnableEvalMapCleanup;
        #ifdef PROFILE_MEM
            Boolean TraceObjectAllocation;
        #endif
        Number Sse;
        Number DeletedPropertyReuseThreshold;
        Boolean ForceStringKeyedSimpleDictionaryTypeHandler;
        Number BigDictionaryTypeHandlerThreshold;
        Boolean TypeSnapshotEnumeration;
        Boolean IsolatePrototypes;
        Boolean ChangeTypeOnProto;
        Boolean ShareInlineCaches;
        Boolean DisableDebugObject;
        Boolean DumpHeap;
        String autoProxy;
        Number PerfHintLevel;
        #ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
            Boolean MemProtectHeap;
        #endif
        #ifdef RECYCLER_STRESS
            Boolean MemProtectHeapStress;
            #if ENABLE_CONCURRENT_GC
                Boolean MemProtectHeapBackgroundStress;
                Boolean MemProtectHeapConcurrentStress;
                Boolean MemProtectHeapConcurrentRepeatStress;
            #endif
            #if ENABLE_PARTIAL_GC
                Boolean MemProtectHeapPartialStress;
            #endif
        #endif
        #ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
            Boolean FixPropsOnPathTypes;
        #endif
        NumberSet BailoutTraceFilter;
        NumberSet RejitTraceFilter;

        // recycler heuristic flags
        Number MaxBackgroundFinishMarkCount;
        Number BackgroundFinishMarkWaitTime;
        Number MinBackgroundRepeatMarkRescanBytes;

        #if defined(_M_X64)
            Boolean ZeroMemoryWithNonTemporalStore;
        #endif

        // recycler memory restrict test flags
        Number MaxMarkStackPageCount;
        Number MaxTrackedObjectListCount;

        // make the recycler page integration path easier to hit
        Number NumberAllocPlusSize;

        #if DBG
            Boolean InitializeInterpreterSlotsWithInvalidStackVar;
        #endif

        #if DBG
            Number PRNGSeed0;
            Number PRNGSeed1;
        #endif

        Boolean ClearInlineCachesOnCollect;
        Number InlineCacheInvalidationListCompactionThreshold;
        Number ConstructorCacheInvalidationThreshold;

        #ifdef IR_VIEWER
            Boolean IRViewer;
        #endif /* IR_VIEWER */

        Number GCMemoryThreshold;

        #if DBG
            Number SimulatePolyCacheWithOneTypeForInlineCacheIndex;
        #endif

        Number JITServerIdleTimeout;
        Number JITServerMaxInactivePageAllocatorCount;

        Boolean StrictWriteBarrierCheck;
        Boolean WriteBarrierTest;
        Boolean ForceSoftwareWriteBarrier;
        Boolean VerifyBarrierBit;
        Boolean EnableBGFreeZero;
        Boolean KeepRecyclerTrackData;

        Number MaxSingleAllocSizeInMB;
// TODO (hanhossain): ConfigFlagsList end

                bool            flagPresent[FlagCount];

#if CONFIG_PARSE_CONFIG_FILE
                // save the jscript.config for easier to get the raw input while analyzing dump file
                char16_t          rawInputFromConfigFile[512];
                int             rawInputFromConfigFileIndex;
#endif

    private:
        //
        // This variable is defined for the constructor
        //
                int             nDummy;

                // indicates whether a flag is a parent flag
                bool            flagIsParent[FlagMax + 1];

    // Implementation
    private:
                void *      GetProperty(Flag flag) const;

                void        SetAllParentFlagsAsDefaultValue();

                // special callback logic
                void        FlagSetCallback_ES6All(Js::Boolean value);
                void        FlagSetCallback_ES6Experimental(Js::Boolean value);

    public:
        void FinalizeConfiguration();
        void EnableExperimentalFlag();

        // Some config flags are expected to be constant during the lifetime of
        // a ScriptContext object. However, some other flags can change,
        // possibly multiple times.
        //
        // To keep the flags constant during the lifetime of a ScriptContext
        // object, we copy the ones that it needs into ThreadContext and have
        // it fetch them from ThreadContext instead of here. Given that a
        // ScriptContext object is bound to a ThreadContext object and never
        // gets reassigned, this keeps the flags constant while the
        // ScriptContext object is alive.
        //
        // Currently, among the flags used by ScriptContext, only the
        // experimental flags are altered after initialization. Therefore, only
        // access to these flags are serialized.
        //
        // Note that this lock is acquired automatically only when the
        // experimental flags are initialized via the EnableExperimentalFlag()
        // method. It should be manually acquired anywhere else where these
        // flags are accessed.
        std::recursive_mutex csExperimentalFlags;

    private:
        void TransferAcronymFlagConfiguration();
        void TranslateFlagConfiguration();
    };


    class Profiler;

    class Configuration
    {
    public:
        ConfigFlagsTable           flags;
        static Configuration        Global;

        // Public in case the client wants to have
        // a separate config from the global one
        Configuration();
    };

//Create macros for a useful subset of the config options that either get the value from the configuration (if the option is enabled) or
//just use the hard coded default value (if not). All the ...IsEnabled(...) default to false.
#define CONFIG_ISENABLED(flag)      (Js::Configuration::Global.flags.IsEnabled(flag))
#define CUSTOM_CONFIG_ISENABLED(flags, flag)      (flags.IsEnabled(flag))
#define CONFIG_FLAG(flag)           (Js::Configuration::Global.flags.##flag)
#define CUSTOM_CONFIG_FLAG(flags, flag) (flags.##flag)
#define CONFIG_FLAG_RELEASE(flag)   CONFIG_FLAG(flag)
#define CONFIG_FLAG_CONTAINS(flag, func)  (Js::Configuration::Global.flags.##flag.Contains((func)->GetLocalFunctionId()))

#define PHASE_OFF_PROFILED_BYTE_CODE(phase, func) Js::Configuration::Global.flags.OffProfiledByteCode.IsEnabled((phase),(func)->GetSourceContextId(),(func)->GetLocalFunctionId())
#define PHASE_OFF_PROFILED_BYTE_CODE_ALL(phase) Js::Configuration::Global.flags.OffProfiledByteCode.IsEnabledForAll((phase))
#define PHASE_OFF_PROFILED_BYTE_CODE_OPTFUNC(phase, func) ((func) ? PHASE_OFF_PROFILED_BYTE_CODE(phase, func) : PHASE_OFF_PROFILED_BYTE_CODE_ALL(phase))

#define PHASE_OFF1(phase)           Js::Configuration::Global.flags.Off.IsEnabled((phase))
#define CUSTOM_PHASE_OFF1(flags, phase)           flags.Off.IsEnabled((phase))
#define PHASE_OFF_ALL(phase)        Js::Configuration::Global.flags.Off.IsEnabledForAll((phase))
#define PHASE_OFF(phase, func)      PHASE_OFF_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_OFF_OPTFUNC(phase, func) ((func) ? PHASE_OFF(phase, func) : PHASE_OFF_ALL(phase))
#define PHASE_OFF_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Off.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_ON1(phase)            Js::Configuration::Global.flags.On.IsEnabled((phase))
#define CUSTOM_PHASE_ON1(flags, phase) flags.On.IsEnabled((phase))
#define PHASE_ON(phase, func)       PHASE_ON_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_ON_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.On.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_FORCE1(phase)         Js::Configuration::Global.flags.Force.IsEnabled((phase))
#define CUSTOM_PHASE_FORCE1(flags, phase) flags.Force.IsEnabled((phase))
#define PHASE_FORCE(phase, func)    PHASE_FORCE_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_FORCE_OPTFUNC(phase, func) ((func) ? PHASE_FORCE(phase, func) : PHASE_FORCE1(phase))
#define PHASE_FORCE_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Force.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_STRESS1(phase)        Js::Configuration::Global.flags.Stress.IsEnabled((phase))
#define PHASE_STRESS(phase, func)   PHASE_STRESS_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_STRESS_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Stress.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_TRACE1(phase)         Js::Configuration::Global.flags.Trace.IsEnabled((phase))
#define CUSTOM_PHASE_TRACE1(flags, phase) flags.Trace.IsEnabled((phase))
#define PHASE_TRACE(phase, func)    PHASE_TRACE_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_TRACE_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Trace.IsEnabled((phase), (sourceId), (functionId))

#if DBG
    // Enabling the ability to trace the StringConcat phase only in debug builds due to performance impact
    #define PHASE_TRACE_StringConcat PHASE_TRACE1(Js::StringConcatPhase)
#else
    #define PHASE_TRACE_StringConcat (false)
#endif

#define PHASE_VERBOSE_TRACE1(phase) \
    ((PHASE_TRACE1((phase))) && Js::Configuration::Global.flags.Verbose)

#define CUSTOM_PHASE_VERBOSE_TRACE1(flags, phase) \
    ((CUSTOM_PHASE_TRACE1((flags), (phase))) && flags.Verbose)

#define PHASE_VERBOSE_TRACE(phase, func) \
    ((PHASE_TRACE((phase), (func))) && Js::Configuration::Global.flags.Verbose)
#define PHASE_VERBOSE_TRACE_RAW(phase, sourceId, functionId) \
    ((PHASE_TRACE_RAW((phase), (sourceId), (functionId))) && Js::Configuration::Global.flags.Verbose)

#define PHASE_DUMP1(phase)          Js::Configuration::Global.flags.Dump.IsEnabled((phase))
#define PHASE_DUMP(phase, func)     Js::Configuration::Global.flags.Dump.IsEnabled((phase), (func)->GetSourceContextId(),(func)->GetLocalFunctionId())

#define PHASE_DEBUGBREAK_ON_PHASE_BEGIN(phase, func) Js::Configuration::Global.flags.DebugBreakOnPhaseBegin.IsEnabled((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())

#define PHASE_STATS1(phase)         Js::Configuration::Global.flags.Stats.IsEnabled((phase))
#define CUSTOM_PHASE_STATS1(flags, phase) flags.Stats.IsEnabled((phase))
#define PHASE_VERBOSE_STATS1(phase) \
    ((PHASE_STATS1(phase)) && Js::Configuration::Global.flags.Verbose)

#define PHASE_STATS_ALL(phase)      Js::Configuration::Global.flags.Stats.IsEnabledForAll((phase))
#define PHASE_STATS(phase, func)    PHASE_STATS_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_STATS_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Stats.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_VERBOSE_STATS(phase, func) \
    ((PHASE_STATS(phase, func)) && Js::Configuration::Global.flags.Verbose)
#define PHASE_VERBOSE_STATS_RAW(phase, sourceId, functionId) \
    ((PHASE_STATS_RAW(phase, sourceId, functionId)) && Js::Configuration::Global.flags.Verbose)

#define PHASE_TESTTRACE1(phase) Js::Configuration::Global.flags.TestTrace.IsEnabled((phase))
#define PHASE_TESTTRACE(phase, func) PHASE_TESTTRACE_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_TESTTRACE_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.TestTrace.IsEnabled((phase), (sourceId), (functionId))
#define PHASE_TESTTRACE1_TELEMETRY(phase) (false)

#define PHASE_PRINT_TRACE1(phase, ...) \
    if (PHASE_TRACE1(phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define CUSTOM_PHASE_PRINT_TRACE1(flags, phase, ...) \
    if (CUSTOM_PHASE_TRACE1(flags, phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_TRACE(phase, func, ...) \
    if (PHASE_TRACE(phase, func)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_TRACE_RAW(phase, sourceId, functionId, ...) \
    if (PHASE_TRACE_RAW(phase, sourceId, functionId)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TRACE1(phase, ...) \
    if (PHASE_VERBOSE_TRACE1(phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define CUSTOM_PHASE_PRINT_VERBOSE_TRACE1(flags, phase, ...) \
    if (CUSTOM_PHASE_VERBOSE_TRACE1(flags, phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TRACE(phase, func, ...) \
    if (PHASE_VERBOSE_TRACE(phase, func)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TRACE_RAW(phase, sourceId, functionId, ...) \
    if (PHASE_VERBOSE_TRACE_RAW(phase, sourceId, functionId)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_VERBOSE_TESTTRACE1(phase) (PHASE_TESTTRACE1(phase) && Js::Configuration::Global.flags.Verbose)
#define PHASE_VERBOSE_TESTTRACE(phase, func) \
    (PHASE_TESTTRACE(phase, func) && Js::Configuration::Global.flags.Verbose)
#define PHASE_VERBOSE_TESTTRACE_RAW(phase, sourceId, functionId) \
    (PHASE_TESTTRACE_RAW(phase, sourceId, functionId) && Js::Configuration::Global.flags.Verbose)

#define PHASE_PRINT_TESTTRACE1(phase, ...) \
    if (PHASE_TESTTRACE1(phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_TESTTRACE(phase, func, ...) \
    if (PHASE_TESTTRACE(phase, func)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_TESTTRACE_RAW(phase, sourceId, functionId, ...) \
    if (PHASE_TESTTRACE_RAW(phase, sourceId, functionId)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TESTTRACE1(phase, ...) \
    if (PHASE_VERBOSE_TESTTRACE1(phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TESTTRACE(phase, func, ...) \
    if (PHASE_VERBOSE_TESTTRACE(phase, func)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TESTTRACE_RAW(phase, sourceId, functionId, ...) \
    if (PHASE_VERBOSE_TESTTRACE_RAW(phase, sourceId, functionId)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#ifdef ENABLE_REGEX_CONFIG_OPTIONS
#define REGEX_CONFIG_FLAG(flag) (Js::Configuration::Global.flags.##flag)
#else
#define REGEX_CONFIG_FLAG(flag) (DEFAULT_CONFIG_##flag)
#endif

#ifdef SUPPORT_INTRUSIVE_TESTTRACES
#define PHASE_PRINT_INTRUSIVE_TESTTRACE1(phase, ...) \
    PHASE_PRINT_TESTTRACE1(phase, __VA_ARGS__)
#else
#define PHASE_PRINT_INTRUSIVE_TESTTRACE1(phase, ...) (false)
#endif

#define PHASE_ENABLED1(phase)       (Js::PhaseIsEnabled::phase())
#define PHASE_ENABLED(phase, func)  (Js::PhaseIsEnabled::phase(func))
#define PHASE_ENABLED_RAW(phase, sourceId, functionId) \
                                    (Js::PhaseIsEnabled::phase(sourceId, functionId))
struct PhaseIsEnabled
{
    static bool DeferParsePhase() { return !PHASE_OFF1(Js::DeferParsePhase); }

    template <typename FUNC>
    static bool DeferParsePhase(FUNC func) { return !PHASE_OFF(Js::DeferParsePhase, func); }

    static bool DeferParsePhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::DeferParsePhase, sourceId, functionId);
    }
    static bool ParallelParsePhase() { return PHASE_ON1(Js::ParallelParsePhase); }

    template <typename FUNC>
    static bool ParallelParsePhase(FUNC func) { return PHASE_ON(Js::ParallelParsePhase, func); }

    static bool ParallelParsePhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return PHASE_ON_RAW(Js::ParallelParsePhase, sourceId, functionId);
    }
    static bool WasmPhase() { return !PHASE_OFF1(Js::WasmPhase); }

    template <typename FUNC>
    static bool WasmPhase(FUNC func) { return !PHASE_OFF(Js::WasmPhase, func); }

    static bool WasmPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::WasmPhase, sourceId, functionId);
    }
    static bool WasmDeferredPhase() { return !PHASE_OFF1(Js::WasmDeferredPhase); }

    template <typename FUNC>
    static bool WasmDeferredPhase(FUNC func) { return !PHASE_OFF(Js::WasmDeferredPhase, func); }

    static bool WasmDeferredPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::WasmDeferredPhase, sourceId, functionId);
    }
    static bool WasmValidatePrejitPhase() { return PHASE_ON1(Js::WasmValidatePrejitPhase); }

    template <typename FUNC>
    static bool WasmValidatePrejitPhase(FUNC func) { return PHASE_ON(Js::WasmValidatePrejitPhase, func); }

    static bool WasmValidatePrejitPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return PHASE_ON_RAW(Js::WasmValidatePrejitPhase, sourceId, functionId);
    }
    static bool AsmjsFunctionEntryPhase() { return PHASE_ON1(Js::AsmjsFunctionEntryPhase); }

    template <typename FUNC>
    static bool AsmjsFunctionEntryPhase(FUNC func) { return PHASE_ON(Js::AsmjsFunctionEntryPhase, func); }

    static bool AsmjsFunctionEntryPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return PHASE_ON_RAW(Js::AsmjsFunctionEntryPhase, sourceId, functionId);
    }
    static bool InlineCallbacksPhase() { return !PHASE_OFF1(Js::InlineCallbacksPhase); }

    template <typename FUNC>
    static bool InlineCallbacksPhase(FUNC func) { return !PHASE_OFF(Js::InlineCallbacksPhase, func); }

    static bool InlineCallbacksPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::InlineCallbacksPhase, sourceId, functionId);
    }
    static bool CaptureByteCodeRegUsePhase() { return !PHASE_OFF1(Js::CaptureByteCodeRegUsePhase); }

    template <typename FUNC>
    static bool CaptureByteCodeRegUsePhase(FUNC func) { return !PHASE_OFF(Js::CaptureByteCodeRegUsePhase, func); }

    static bool CaptureByteCodeRegUsePhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::CaptureByteCodeRegUsePhase, sourceId, functionId);
    }
};

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
///
/// class RangeBase
///
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------

template <typename TRangeUnitData>
void
RangeBase<TRangeUnitData>::Add(TRangeUnitData i, RangeBase<TRangeUnitData>* oppositeRange)
{
    Add(i, i, oppositeRange);
}

template <typename TRangeUnitData>
void
RangeBase<TRangeUnitData>::Add(TRangeUnitData i, TRangeUnitData j, RangeBase<TRangeUnitData>* oppositeRange)
{
    RangeUnit<TRangeUnitData> a(i, j);
    range.Prepend(a);
    if (oppositeRange)
    {
        if (oppositeRange->range.Empty())
        {
            oppositeRange->range.Prepend(GetFullRange<TRangeUnitData>());
        }
        // Do an intersection
        auto it = oppositeRange->range.GetEditingIterator();
        while (it.Next())
        {
            Unit& unit = it.Data();
            bool c1 = RangeUnitContains(unit, i);
            bool c2 = RangeUnitContains(unit, j);
            bool c3 = RangeUnitContains(a, unit.i);
            bool c4 = RangeUnitContains(a, unit.j);
            enum IntersectionCase
            {
                NoIntersection = 0,
                AddedFullyContained = 3, // [ u [a] ]
                IntersectionAdded_Unit = 6, // [ a [ ] u ]
                CommonLowBound_Unit = 7, // [[ a ] u ]
                IntersectionUnit_Added = 9, // [ u [ ] a ]
                CommonTopBound_Unit = 11, // [ u [ a ]]
                UnitFullyContained = 12, // [ a [u] ]
                CommonLowBound_Added = 13, // [[ u ] a ]
                CommonTopBound_Added = 14, // [ a [ u ]]
                FullIntersection = 15 // [[a, u]]
            };
            IntersectionCase intersectionCase = (IntersectionCase)((int)c1 | (c2 << 1) | (c3 << 2) | (c4 << 3));
            switch (intersectionCase)
            {
            case NoIntersection:
                // Nothing to do
                break;
            case AddedFullyContained:
            {
                // Need to break the current in 2
                Unit lowUnit = unit;
                Unit topUnit = unit;
                lowUnit.j = GetPrevious(a.i);
                topUnit.i = GetNext(a.j);
                it.InsertBefore(lowUnit);
                it.InsertBefore(topUnit);
                it.RemoveCurrent();
                break;
            }
            case IntersectionAdded_Unit:
            case CommonLowBound_Unit:
                // Move the unit lower bound after the added upper bound
                unit.i = GetNext(a.j);
                break;
            case IntersectionUnit_Added:
            case CommonTopBound_Unit:
                // Move the unit upper bound before the added lower bound
                unit.j = GetPrevious(a.i);
                break;
            case CommonTopBound_Added:
            case CommonLowBound_Added:
            case UnitFullyContained:
            case FullIntersection:
                // Remove the unit
                it.RemoveCurrent();
                break;
            default:
                Assert(UNREACHED);
                break;
            }
        }
    }
}

template <typename TRangeUnitData>
bool
RangeBase<TRangeUnitData>::ContainsAll()
{
    return range.Empty();
}

template <typename TRangeUnitData>
void
Js::RangeBase<TRangeUnitData>::Clear()
{
    range.Clear();
}

///----------------------------------------------------------------------------
///
/// RangeBase::InRange
///
/// Searches for each element in the list of UnitRanges. If the given integer
/// is between the 2 values, then return true; If no element is present in range
/// then, then we return true
///
///----------------------------------------------------------------------------

template <typename TRangeUnitData>
bool RangeBase<TRangeUnitData>::InRange(TRangeUnitData n)
{
    if (range.Empty())
    {
        return true;
    }
    else
    {
        return range.MapUntil([n](RangeUnit<TRangeUnitData> const& unit)
        {
            return RangeUnitContains(unit, n);
        });
    }
}
}  // namespace Js

