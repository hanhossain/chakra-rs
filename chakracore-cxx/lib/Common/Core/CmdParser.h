//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>
#include "Core/ICustomConfigFlags.h"

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
///
/// class CmdLineArgsParser
///
/// Parses the following grammar
///
///      range      = integer | integer - integer | range,range
///      parameter  = integer | string  | phase[:range]
///      flag       = string
///      phase      = string
///      [-|/]flag[:parameter]
///
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------

class CmdLineArgsParser : private ICmdLineArgsParser
{
// Data
private:
    static const int  MaxTokenSize  = 512;

    Js::ConfigFlagsTable& flagTable;
    const char16_t *pszCurrentArg;
    ICustomConfigFlags * pCustomConfigFlags;

// Methods
public:
    int Parse(const std::vector<std::u16string> &vargs);
    int Parse(const char16_t* token) throw();
    CmdLineArgsParser(ICustomConfigFlags * pCustomConfigFlags = nullptr, Js::ConfigFlagsTable& flagTable = Js::Configuration::Global.flags);
    ~CmdLineArgsParser();

// Helper Classes
private:

    ///----------------------------------------------------------------------------
    ///
    /// class Exception
    ///
    ///----------------------------------------------------------------------------

    class Exception {
        const char16_t*        pszMsg;
    public:
        Exception(const char16_t* message):
            pszMsg(message)
        {}

        operator const char16_t* () const
        {
            return this->pszMsg;
        }
    };

// Implementation
private:
            bool                       ParseBoolean();
            char16_t*                     ParseString(__inout_ecount(ceBuffer) char16_t* buffer, size_t ceBuffer = MaxTokenSize, bool fTreatColonAsSeparator = true);
            int                        ParseInteger();
            Js::SourceFunctionNode     ParseSourceFunctionIds();
            void                       ParsePhase(Js::Phases *pPhase, Js::Phases *oppositePhase);
            void                       ParseRange(Js::Range *range, Js::Range *oppositeRange);
            void                       ParseNumberRange(Js::NumberRange *range);
            void                       ParseFlag();
            void                       ParseNumberSet(Js::NumberSet * numberSet);
            void                       ParseNumberPairSet(Js::NumberPairSet * numberPairSet);
            void                       ParseNumberTrioSet(Js::NumberTrioSet * numberTrioSet);
            void                       PrintUsage();

            char16_t CurChar() const
            {
                return this->pszCurrentArg[0];
            }

            char16_t PeekChar() const
            {
                return this->pszCurrentArg[1];
            }

            void NextChar()
            {
                this->pszCurrentArg++;
            }

            bool IsDigit() const {
                return (CurChar() >='0' && CurChar() <= '9');
            }

            bool IsHexDigit() const
            {
                return (CurChar() >= '0' && CurChar() <= '9') ||
                    (CurChar() >= 'A' && CurChar() <= 'F') ||
                    (CurChar() >= 'a' && CurChar() <= 'f');
            }

            // Implements ICmdLineArgsParser
            virtual BSTR GetCurrentString() override;
            virtual bool GetCurrentBoolean() override
            {
                return ParseBoolean();
            }
            virtual int GetCurrentInt() override
            {
                NextChar();
                return  ParseInteger();
            }
};
