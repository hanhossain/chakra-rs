//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

namespace Js
{
    template<typename T>
    inline T AsmJsMath::Add( T aLeft, T aRight )
    {
        return aLeft + aRight;
    }

    template<typename T>
    inline T AsmJsMath::Sub( T aLeft, T aRight )
    {
        return aLeft - aRight;
    }

    template<typename T> inline int AsmJsMath::CmpLt( T aLeft, T aRight ){return (int)(aLeft <  aRight);}
    template<typename T> inline int AsmJsMath::CmpLe( T aLeft, T aRight ){return (int)(aLeft <= aRight);}
    template<typename T> inline int AsmJsMath::CmpGt( T aLeft, T aRight ){return (int)(aLeft >  aRight);}
    template<typename T> inline int AsmJsMath::CmpGe( T aLeft, T aRight ){return (int)(aLeft >= aRight);}
    template<typename T> inline int AsmJsMath::CmpEq( T aLeft, T aRight ){return (int)(aLeft == aRight);}
    template<typename T> inline int AsmJsMath::CmpNe( T aLeft, T aRight ){return (int)(aLeft != aRight);}

    template<typename T> 
    inline T AsmJsMath::And( T aLeft, T aRight )
    {
        return aLeft & aRight;
    }

    template<typename T> 
    inline T AsmJsMath::Or( T aLeft, T aRight )
    {
        return aLeft | aRight;
    }

    template<typename T> 
    inline T AsmJsMath::Xor( T aLeft, T aRight )
    {
        return aLeft ^ aRight;
    }

    template<typename T> 
    inline T AsmJsMath::Shl( T aLeft, T aRight )
    {
        return aLeft << aRight;
    }

    template<typename T> 
    inline T AsmJsMath::Shr( T aLeft, T aRight )
    {
        return aLeft >> aRight;
    }

    template<typename T> 
    inline T AsmJsMath::ShrU( T aLeft, T aRight )
    {
        return aLeft >> aRight;
    }

    template<typename T>
    inline T AsmJsMath::Neg( T aLeft )
    {
        return -aLeft;
    }
}
