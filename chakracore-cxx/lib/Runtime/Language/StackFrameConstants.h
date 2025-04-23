#pragma once

#include <stdint.h>

struct StackFrameConstants
{
#if defined(_M_IX86)
    static const size_t StackCheckCodeHeightThreadBound = 35;
    static const size_t StackCheckCodeHeightNotThreadBound = 47;
    static const size_t StackCheckCodeHeightWithInterruptProbe = 53;
#elif defined(_M_X64)
    static const size_t StackCheckCodeHeightThreadBound = 57;
    static const size_t StackCheckCodeHeightNotThreadBound = 62;
    static const size_t StackCheckCodeHeightWithInterruptProbe = 68;
#elif defined(_M_ARM)
    static const size_t StackCheckCodeHeight = 64;
    static const size_t StackCheckCodeHeightThreadBound = StackFrameConstants::StackCheckCodeHeight;
    static const size_t StackCheckCodeHeightNotThreadBound = StackFrameConstants::StackCheckCodeHeight;
    static const size_t StackCheckCodeHeightWithInterruptProbe = StackFrameConstants::StackCheckCodeHeight;
#elif defined(_M_ARM64)
    static const size_t StackCheckCodeHeight = 58*2;
    static const size_t StackCheckCodeHeightThreadBound = StackFrameConstants::StackCheckCodeHeight;
    static const size_t StackCheckCodeHeightNotThreadBound = StackFrameConstants::StackCheckCodeHeight;
    static const size_t StackCheckCodeHeightWithInterruptProbe = StackFrameConstants::StackCheckCodeHeight;
#endif
};
