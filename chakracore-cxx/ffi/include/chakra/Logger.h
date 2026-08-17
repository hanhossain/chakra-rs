#pragma once
#include <format>
#include <source_location>
#include <string>
#include <rust/cxx.h>

namespace chakra
{

    class Logger
    {
    public:
        static void error(const std::string &message,
                          const std::source_location &location = std::source_location::current());
        static void warn(const std::string &message,
                         const std::source_location &location = std::source_location::current());
        static void info(const std::string &message,
                         const std::source_location &location = std::source_location::current());
        static void debug(const std::string &message,
                          const std::source_location &location = std::source_location::current());
        static void trace(const std::string &message,
                          const std::source_location &location = std::source_location::current());
    };

} // namespace chakra

template<>
struct std::formatter<rust::String> : std::formatter<std::string_view>
{
    auto format(const rust::String &string, std::format_context &ctx) const
    {
        auto temp = static_cast<std::string_view>(string);
        return std::formatter<std::string_view>::format(temp, ctx);
    }
};
