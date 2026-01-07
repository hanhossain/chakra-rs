//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    /***************************************************************************
        Big non-negative integer class.
    ***************************************************************************/
    class BigUInt
    {
        // Non-negative BigInt is stored as an array of 'digit' where each digit is unit32
    private:
        // Make this big enough that we rarely have to call malloc.
        enum { kcluMaxInit = 30 };// initilize 30 digits

        int32_t m_cluMax; // current maximum length (or number of digits) it can contains
        int32_t m_clu; // current length (or number of digits)
        uint32_t *m_prglu; // pointer to array of digits
        uint32_t m_rgluInit[kcluMaxInit]; // pre-defined space to store array

        inline BigUInt & operator= (BigUInt &bi);
        bool FResize(int32_t clu);// allocate more space if length go over maximum

#if DBG
        #define AssertBi(pbi) Assert(pbi); (pbi)->AssertValid(true);
        #define AssertBiNoVal(pbi) Assert(pbi); (pbi)->AssertValid(false);
        inline void AssertValid(bool fCheckVal);
#else //!DBG
        #define AssertBi(pbi)
        #define AssertBiNoVal(pbi)
#endif //!DBG

    public:
        BigUInt(void);
        ~BigUInt(void);

        bool FInitFromRglu(uint32_t *prglu, int32_t clu); // init from array and length
        bool FInitFromBigint(BigUInt *pbiSrc); 
        template <typename EncodedChar>
        bool FInitFromDigits(const EncodedChar *prgch, int32_t cch, int32_t *pcchDec); // init from char of digits
        bool FMulAdd(uint32_t luMul, uint32_t luAdd);
        bool FMulPow5(int32_t c5);
        bool FShiftLeft(int32_t cbit);
        void ShiftLusRight(int32_t clu);
        void ShiftRight(int32_t cbit);
        int Compare(BigUInt *pbi);
        bool FAdd(BigUInt *pbi);
        void Subtract(BigUInt *pbi);
        int DivRem(BigUInt *pbi);

        int32_t Clu(void); // return current length
        uint32_t Lu(int32_t ilu); // return digit at position ilu start from 0
        double GetDbl(void);
    };
}
