//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:



--*/

#ifndef _CORUNIX_INL
#define _CORUNIX_INL

#include "corunix.hpp"
#include "dbgmsg.h"

namespace CorUnix
{

    bool CAllowedObjectTypes::IsTypeAllowed(PalObjectTypeId eTypeId)
    {
        _ASSERTE(eTypeId != ObjectTypeIdCount);
        return m_rgfAllowedTypes[eTypeId];
    };

    CAllowedObjectTypes::CAllowedObjectTypes(
        PalObjectTypeId rgAllowedTypes[],
        uint32_t dwAllowedTypeCount
    )
    {
        ZeroMemory(m_rgfAllowedTypes, sizeof(m_rgfAllowedTypes));
        for (uint32_t dw = 0; dw < dwAllowedTypeCount; dw += 1)
        {
            _ASSERTE(rgAllowedTypes[dw] != ObjectTypeIdCount);
            m_rgfAllowedTypes[rgAllowedTypes[dw]] = TRUE;
        }
    };

    CAllowedObjectTypes::CAllowedObjectTypes(
       PalObjectTypeId eAllowedType
       )
    {
        ZeroMemory(m_rgfAllowedTypes, sizeof(m_rgfAllowedTypes));

        _ASSERTE(eAllowedType != ObjectTypeIdCount);
        m_rgfAllowedTypes[eAllowedType] = TRUE;
    };
}

#endif // _CORUNIX_H

