#include "chakra/Logger.h"
#include "chakracore-sys/src/logger.rs.h"

namespace chakra
{
    void Logger::error(const std::string &message, const std::source_location &location)
    {
        chakra_rs::log::error(location.function_name(), location.file_name(), location.line(), message);
    }
} // namespace chakra
