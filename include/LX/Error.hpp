#pragma once

#include <format>
#include <string>
#include <LX/LX.hpp>

namespace LX
{
    [[noreturn]] void Error(const std::string&);
    [[noreturn]] void Error(const SourceLocation&, const std::string&);

    template <typename... Args>
    [[noreturn]] void Error(const std::string& format, Args&&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        Error(message);
    }

    template <typename... Args>
    [[noreturn]] void Error(const SourceLocation& where, const std::string& format, Args&&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        Error(where, message);
    }

    struct CompileError : std::exception
    {
        explicit CompileError(std::string);

        [[nodiscard]] const char* what() const noexcept override;

        std::string Message;
    };
}
