#pragma once

#include <format>
#include <map>
#include <string>
#include <LX/SourceLocation.hpp>

namespace LX
{
    enum TokenType
    {
        TokenType_EOF,

        TokenType_BraceClose,
        TokenType_BraceOpen,
        TokenType_BracketClose,
        TokenType_BracketOpen,
        TokenType_Char,
        TokenType_Colon,
        TokenType_ColonEqual,
        TokenType_Comma,
        TokenType_Dollar,
        TokenType_Float,
        TokenType_Int,
        TokenType_Operator,
        TokenType_ParenClose,
        TokenType_ParenOpen,
        TokenType_Period,
        TokenType_Semicolon,
        TokenType_String,
        TokenType_Symbol,
    };

    struct Token
    {
        SourceLocation Where;
        TokenType Type = TokenType_EOF;

        std::string StringValue;
        size_t IntegerValue = 0;
        double FloatValue = 0.f;
    };
}

namespace std
{
    template <>
    struct formatter<LX::TokenType> : formatter<string>
    {
        template <typename FormatContext>
        auto format(const LX::TokenType& type, FormatContext& ctx) const
        {
            static std::map<LX::TokenType, std::string> names
            {
                {LX::TokenType_EOF, "EOF"},
                {LX::TokenType_Symbol, "Symbol"},
                {LX::TokenType_Int, "Int"},
                {LX::TokenType_Float, "Float"},
                {LX::TokenType_Operator, "Operator"},
                {LX::TokenType_String, "String"},
            };
            return formatter<string>::format(names[type], ctx);
        }
    };

    template <>
    struct formatter<LX::Token> : formatter<string>
    {
        template <typename FormatContext>
        auto format(const LX::Token& token, FormatContext& ctx) const
        {
            return format_to(
                ctx.out(),
                "{{ {}, '{}' }}",
                token.Type,
                token.StringValue);
        }
    };
}
