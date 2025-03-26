//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCommonPch.h"
#include "Common/SmartFpuControl.h"

//
// Floating point unit utility functions
//

static errno_t SetFPUControlDefault(void)
{
    return 0;
}

static errno_t GetFPUControl(unsigned int *pctrl)
{
    Assert(pctrl != nullptr);
    return 0;
}

static errno_t SetFPUControl(unsigned int fpctrl)
{
    return 0;
}

static void ClearFPUStatus(void)
{
}

template <bool enabled>
SmartFPUControlT<enabled>::SmartFPUControlT()
{
    if (enabled)
    {
        m_oldFpuControl = INVALID_FPUCONTROL;
        ClearFPUStatus(); // Clear pending exception status first (blue 555235)
        m_err = GetFPUControl(&m_oldFpuControl);
        if (m_err == 0)
        {
            m_err = SetFPUControlDefault();
        }
    }
#if DBG
    else
    {
        m_oldFpuControl = INVALID_FPUCONTROL;
        m_err = GetFPUControl(&m_oldFpuControl);
        m_oldFpuControlForConsistencyCheck = m_oldFpuControl;
    }
#endif
}

template <bool enabled>
SmartFPUControlT<enabled>::~SmartFPUControlT()
{
    if (enabled)
    {
        RestoreFPUControl();
    }
#if DBG
    else
    {
        uint currentFpuControl;
        m_err = GetFPUControl(&currentFpuControl);
        if (m_err == 0 && m_oldFpuControlForConsistencyCheck != INVALID_FPUCONTROL)
        {
            Assert(m_oldFpuControlForConsistencyCheck == currentFpuControl);
        }
    }
#endif
}

template <bool enabled>
void
SmartFPUControlT<enabled>::RestoreFPUControl()
{
    if (enabled)
    {
        if (m_oldFpuControl != INVALID_FPUCONTROL)
        {
            m_err = SetFPUControl(m_oldFpuControl);
            m_oldFpuControl = INVALID_FPUCONTROL; // Only restore once
        }
    }
    else
    {
        // Shouldn't restore if this is not enabled
        Assert(false);
    }
}

// Explicit instantiation
template class SmartFPUControlT<true>;
template class SmartFPUControlT<false>;
