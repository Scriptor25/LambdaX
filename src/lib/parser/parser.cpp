#include <iostream>
#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

void LX::Parser::Parse(
    Context& ctx,
    std::istream& stream,
    const std::string& filename,
    const Consumer& consumer,
    const bool is_imported)
{
    for (Parser parser(ctx, stream, filename, is_imported); !parser.AtEOF();)
        if (auto ptr = parser.Parse())
            consumer(std::move(ptr));
}

LX::Parser::Parser(Context& ctx, std::istream& stream, const std::string& filename, const bool is_imported)
    : m_IsImported(is_imported), m_Ctx(ctx), m_Stream(stream), m_Where(filename)
{
    m_Tok = m_Stream.get();
    Next();
}

bool LX::Parser::AtEOF() const
{
    return m_Token.Type == TokenType_EOF;
}

bool LX::Parser::At(const TokenType type) const
{
    return m_Token.Type == type;
}

bool LX::Parser::At(const std::string& value) const
{
    return m_Token.StringValue == value;
}

bool LX::Parser::NextAt(const TokenType type)
{
    if (At(type))
    {
        Next();
        return true;
    }
    return false;
}

bool LX::Parser::NextAt(const std::string& value)
{
    if (At(value))
    {
        Next();
        return true;
    }
    return false;
}

LX::Token LX::Parser::Skip()
{
    auto token = m_Token;
    Next();
    return token;
}

LX::Token LX::Parser::Expect(const TokenType type)
{
    if (At(type))
        return Skip();
    Error(m_Token.Where, "expected type {}, got {}", type, m_Token);
}

LX::Token LX::Parser::Expect(const std::string& value)
{
    if (At(value))
        return Skip();
    Error(m_Token.Where, "expected value '{}', got {}", value, m_Token);
}
