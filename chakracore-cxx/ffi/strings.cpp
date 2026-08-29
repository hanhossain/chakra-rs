#include "chakra/strings.h"
#include "chakracore-sys/src/str_helper.rs.h"

namespace chakra
{
    std::u16string to_u16string(const rust::Str str)
    {
        uint16_t *ptr = chakra_rs::str_helper::to_raw_u16_str(str);
        std::u16string result{reinterpret_cast<const char16_t *>(ptr)};
        chakra_rs::str_helper::free_raw_str(ptr);
        return result;
    }
}