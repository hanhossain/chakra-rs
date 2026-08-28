#include "chakra/Logger.h"
#include "chakracore-sys/src/logger.rs.h"

namespace chakra
{
    void Logger::error(const std::string &message, const std::source_location &location)
    {
        chakra_rs::log::error(location.function_name(), location.file_name(), location.line(), message);
    }

    void Logger::warn(const std::string &message, const std::source_location &location)
    {
        chakra_rs::log::warn(location.function_name(), location.file_name(), location.line(), message);
    }

    void Logger::info(const std::string &message, const std::source_location &location)
    {
        chakra_rs::log::info(location.function_name(), location.file_name(), location.line(), message);
    }

    void Logger::debug(const std::string &message, const std::source_location &location)
    {
        chakra_rs::log::debug(location.function_name(), location.file_name(), location.line(), message);
    }

    void Logger::trace(const std::string &message, const std::source_location &location)
    {
        chakra_rs::log::trace(location.function_name(), location.file_name(), location.line(), message);
    }

    Span::Span(rust::Box<chakra_rs::log::ForeignSpan> foreign_span) : foreign_span_(std::move(foreign_span)) {}

    Span Span::create(const std::string &name)
    {
        auto span = chakra_rs::log::ForeignSpan::create(name);
        return Span{std::move(span)};
    }
} // namespace chakra
