#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParsePrimary()
{
    if (At("mut"))
        return ParseMut();

    if (At("switch"))
        return ParseSwitch();

    if (At("sizeof"))
        return ParseSizeOf();

    if (At("nameof"))
        return ParseNameOf();

    if (At("new"))
        return ParseNew();

    if (At("del"))
        return ParseDel();

    if (At(TokenType_ParenOpen))
        return ParseSequence();

    if (At(TokenType_BraceOpen))
        return ParseConstStruct();

    if (At(TokenType_Dollar))
        return ParseConstFunction();

    if (At(TokenType_Operator))
        return ParseUnary();

    if (At(TokenType_Symbol))
        return ParseSymbol();

    if (At(TokenType_Int))
        return std::make_unique<ConstIntExpr>(m_Token.Where, Skip().IntegerValue);

    if (At(TokenType_Float))
        return std::make_unique<ConstFloatExpr>(m_Token.Where, Skip().FloatValue);

    if (At(TokenType_String))
    {
        auto [where, type_, value, int_, float_] = Skip();
        while (At(TokenType_String))
            value += Skip().StringValue;
        return std::make_unique<ConstStringExpr>(where, value);
    }

    if (At(TokenType_Char))
        return std::make_unique<ConstCharExpr>(m_Token.Where, Skip().StringValue[0]);

    Error(m_Token.Where, "unhandled token '{}'", m_Token);
}
