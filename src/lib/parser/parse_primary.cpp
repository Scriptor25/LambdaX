#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParsePrimary()
{
    if (At("("))
        return ParseSequence();

    if (At("{"))
        return ParseConstStruct();

    if (At("$"))
        return ParseConstFunction();

    if (At("mut"))
        return ParseMut();

    if (At(TokenType_Operator))
        return ParseUnary();

    if (At(TokenType_Symbol))
        return ParseSymbol();

    if (At(TokenType_Int))
        return std::make_unique<ConstIntExpr>(m_Token.Where, Skip().IntegerValue);

    if (At(TokenType_Float))
        return std::make_unique<ConstFloatExpr>(m_Token.Where, Skip().FloatValue);

    if (At(TokenType_String))
        return std::make_unique<ConstStringExpr>(m_Token.Where, Skip().StringValue);

    Error(m_Token.Where, "unhandled token '{}'", m_Token);
}
