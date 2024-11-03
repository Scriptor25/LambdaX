#include <iostream>
#include <LX/Error.hpp>
#include <LX/SourceLocation.hpp>

void LX::Error(const std::string& message)
{
    throw CompileError(message);
}

void LX::Error(const SourceLocation& where, const std::string& message)
{
    const auto msg = std::format("at {}:{}:{}: {}", where.Filename, where.Row, where.Col, message);
    throw CompileError(msg);
}

LX::CompileError::CompileError(std::string message)
    : Message(std::move(message))
{
}

const char* LX::CompileError::what() const noexcept
{
    return Message.c_str();
}
