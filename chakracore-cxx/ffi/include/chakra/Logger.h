#pragma once
#include <source_location>
#include <string>

namespace chakra
{

    class Logger
    {
    public:
        static void error(const std::string &message,
                          const std::source_location &location = std::source_location::current());
    };

} // namespace chakra
