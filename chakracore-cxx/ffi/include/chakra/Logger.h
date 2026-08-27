#pragma once
#include <chakracore-sys/src/logger.rs.h>
#include <format>
#include <rust/cxx.h>
#include <source_location>
#include <string>

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

    class Span
    {
    private:
        rust::Box<chakra_rs::log::ForeignSpan> foreign_span_;
        explicit Span(rust::Box<chakra_rs::log::ForeignSpan> foreign_span);
    public:
        static Span create(const std::source_location &location = std::source_location::current());
    };

} // namespace chakra

template <>
struct std::formatter<rust::String> : std::formatter<std::string_view>
{
    auto format(const rust::String &string, std::format_context &ctx) const
    {
        const auto temp = static_cast<std::string_view>(string);
        return std::formatter<std::string_view>::format(temp, ctx);
    }
};
