//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
#if DBG && defined(ENABLE_SCRIPT_DEBUGGING)
    enum DebuggerMode : unsigned int;
#endif

    class FunctionExecutionStateMachine
    {
    public:
        FunctionExecutionStateMachine();
        void InitializeExecutionModeAndLimits(FunctionBody* functionBody);
        void ReinitializeExecutionModeAndLimits(FunctionBody* functionBody);

        // Public Getters and Setters
        ExecutionMode GetExecutionMode() const;
        void SetDefaultInterpreterExecutionMode();
        void SetAsmJsExecutionMode();

        // Interpreter-related functions
        ExecutionMode GetInterpreterExecutionMode(const bool isPostBailout);
        bool IsInterpreterExecutionMode() const;
        uint16 GetProfiledIterations() const;
        uint32_t GetInterpretedCount() const { return interpretedCount; }
        uint32_t SetInterpretedCount(uint32_t val) { return interpretedCount = val; }
        uint32_t IncreaseInterpretedCount() { return interpretedCount++; }
        bool InterpretedSinceCallCountCollection() const;
        void CollectInterpretedCounts();
        void CommitExecutedIterations();

        // JIT-relatedfunctions
        void SetIsSpeculativeJitCandidate();
        uint16 GetSimpleJitLimit() const { return simpleJitLimit; }
        void ResetSimpleJitLimit();
        uint16 GetSimpleJitExecutedIterations() const;
        void SetSimpleJitCallCount(const uint16 simpleJitLimit) const;
        void SetFullJitThreshold(const uint16 newFullJitThreshold, const bool skipSimpleJit = false);
        uint16 GetFullJitThreshold() const { return fullJitThreshold; }
        void SetFullJitRequeueThreshold(const uint16 newFullJitRequeueThreshold);

        // Transition functions
        bool TryTransitionToNextExecutionMode();
        void TryTransitionToNextInterpreterExecutionMode();
        bool TryTransitionToJitExecutionMode();
        void TransitionToSimpleJitExecutionMode();
        void TransitionToFullJitExecutionMode();
        
        // Debug functions
        void PrintLimits() const;
        void AssertIsInitialized() const;

    private:
        // This enum creates a linear view of the progression of ExecutionModes, as
        // described in the banner comment for TryTransitionToNextExecutionMode. Each
        // state has an associated member variable for the state's limit.
        // For more information about ExecutionModes, see ExecutionModes.h.
        enum class ExecutionState : uint8_t
        {
            Interpreter,
            AutoProfilingInterpreter0,
            ProfilingInterpreter0,
            AutoProfilingInterpreter1,
            SimpleJit,
            ProfilingInterpreter1,
            FullJit
        };


        ExecutionState ModeToState(ExecutionMode mode)   const;
        ExecutionMode  StateToMode(ExecutionState state) const;

        uint16& GetStateLimit(ExecutionState state);
        bool IsTerminalState(ExecutionState state);
        void SetExecutionState(ExecutionState state);
        ExecutionState GetDefaultInterpreterExecutionState() const;

        void CommitExecutedIterations(uint16 &limit, const uint executedIterations);
        
        void VerifyExecutionMode(const ExecutionMode executionMode) const;
        void VerifyExecutionModeLimits() const;
        
        // This state machine should be a member of this owner FunctionBody
        WriteBarrierFieldTypeTraits<FunctionBody*>::Type owner;

        // Tracks the current execution state. See ExecutionModes.h for more info.
        WriteBarrierFieldTypeTraits<ExecutionState>::Type executionState;

        // Each of the following limits below is decremented when transitioning from its related mode:
        // Number of times to run interpreter (no profiling) before advancing to next mode
        WriteBarrierFieldTypeTraits<uint16>::Type interpreterLimit;
        // Number of times to run interpreter (min profiling) before advancing to next mode
        WriteBarrierFieldTypeTraits<uint16>::Type autoProfilingInterpreter0Limit;
        // Number of times to run interpreter (full profiling) before advancing to next mode
        WriteBarrierFieldTypeTraits<uint16>::Type profilingInterpreter0Limit;
        // Number of times to run interpreter (min profiling) after already running min and full profiling
        WriteBarrierFieldTypeTraits<uint16>::Type autoProfilingInterpreter1Limit;
        // Number of times to run simple JIT before advancing to next mode
        WriteBarrierFieldTypeTraits<uint16>::Type simpleJitLimit;
        // Number of times to run interpreter (full profiling) before advancing to next mode
        WriteBarrierFieldTypeTraits<uint16>::Type profilingInterpreter1Limit;

        // Total limit to run in non-full JIT execution mode. Typically the sum of the other limits
        WriteBarrierFieldTypeTraits<uint16>::Type fullJitThreshold;
        // Number of attempts to schedule FullJIT until it becomes forced
        WriteBarrierFieldTypeTraits<uint16>::Type fullJitRequeueThreshold;
        // Total number of times this function has run under the interpreter with full profiling
        WriteBarrierFieldTypeTraits<uint16>::Type committedProfiledIterations;
        // Number of times this function has run under the interpreter in the current execution mode
        WriteBarrierFieldTypeTraits<uint32_t>::Type interpretedCount;
        // Used to detect when interpretedCount changed from a particular call
        WriteBarrierFieldTypeTraits<uint32_t>::Type lastInterpretedCount;

        inline uint16 GetDefaultAutoProfilingInterpreter0Limit(bool isCoroutine) const;
        inline uint16 GetDefaultProfilingInterpreter0Limit(bool isCoroutine) const;
        inline uint16 GetDefaultAutoProfilingInterpreter1Limit(bool isCoroutine) const;
        inline uint16 GetDefaultSimpleJitLimit(bool isCoroutine) const;
        inline uint16 GetDefaultProfilingInterpreter1Limit(bool isCoroutine) const;

        inline uint16 GetDefaultFullJitThreshold(bool isCoroutine) const;

#if DBG
        WriteBarrierFieldTypeTraits<bool>::Type initializedExecutionModeAndLimits;
        // Temporary debug flags for automation
        WriteBarrierFieldTypeTraits<bool>::Type hasBeenReinitialized;
#ifdef ENABLE_SCRIPT_DEBUGGING
        WriteBarrierFieldTypeTraits<Js::DebuggerMode>::Type initDebuggerMode;
        WriteBarrierFieldTypeTraits<Js::DebuggerMode>::Type reinitDebuggerMode;
#endif
#endif
    };
};
