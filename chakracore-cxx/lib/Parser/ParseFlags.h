//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

// Parse flags
enum
{
    fscrNil = 0,
    // Unused = 1 << 0,

    // call should return the last expression
    fscrReturnExpression = 1 << 1,
    // 'this.' is optional (for Call)
    fscrImplicitThis = 1 << 2,
    // Heuristically choosing to defer parsing of functions
    fscrWillDeferFncParse = 1 << 3,
    // Functionally able to defer parsing of functions
    fscrCanDeferFncParse = 1 << 4,
    // The code is being generated dynamically (eval, new Function, etc.)
    fscrDynamicCode = 1 << 5,
    fscrUseStrictMode = 1 << 6,
    // same as Opt NoConnect at start of block
    fscrNoImplicitHandlers = 1 << 7,
    // The parser should expose parser state information on the parse nodes.
    // This parser state includes the set of names which are captured by each function
    // and is stored in ParseNodeFnc::capturedNames.
    fscrCreateParserState = 1 << 8,

#if DEBUG
    // used together with fscrReturnExpression
    // enforces JSON semantics in the parsing.
    fscrEnforceJSON = 1 << 9,
#endif

    // this expression has eval semantics (i.e., run in caller's context
    fscrEval = 1 << 10,
    // this is an eval expression
    fscrEvalCode = 1 << 11,
    // this is a global script
    fscrGlobalCode = 1 << 12,
    // Current code should be parsed as a module body
    fscrIsModuleCode = 1 << 13,
    // Disable generation of asm.js code
    fscrNoAsmJs = 1 << 14,
    // ignore prejit global flag
    fscrNoPreJit = 1 << 15,
    // Allow creation of function proxies instead of function bodies
    fscrAllowFunctionProxy = 1 << 16,
    // Current code is engine library code written in Javascript
    fscrIsLibraryCode = 1 << 17,
    // Do not defer parsing
    fscrNoDeferParse = 1 << 18,
    // Current code is a JS built in code written in JavaScript
    fscrJsBuiltIn = 1 << 19,

    // Throw a ReferenceError when the global 'this' is used (possibly in a lambda),
    // for debugger when broken in a lambda that doesn't capture 'this'
    fscrDebuggerErrorOnGlobalThis = 1 << 21,
    //  The eval string is console eval or debugEval, used to have top level
    //  let/const in global scope instead of eval scope so that they can be preserved across console inputs
    fscrConsoleScopeEval = 1 << 22,

    // the function we are parsing is deferred
    fscrDeferredFnc = 1 << 23,
    // the function decl node we deferred is an expression,
    // i.e., not a declaration statement
    fscrDeferredFncExpression = 1 << 24,
    fscrDeferredFncIsAsync = 1 << 25,
    fscrDeferredFncIsMethod = 1 << 26,
    fscrDeferredFncIsGenerator = 1 << 27,
    fscrDeferredFncIsClassMember = 1 << 28,
    fscrDeferredFncIsClassConstructor = 1 << 29,
    fscrDeferredFncIsBaseClassConstructor = 1 << 30,
    fscrAll = (1 << 29) - 1
};
