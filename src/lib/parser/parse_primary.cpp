#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParsePrimary()
{
    if (NextAt("("))
    {
        auto expr = ParseExpr();
        Expect(")");
        return expr;
    }

    if (At(TokenType_Symbol))
    {
        const auto name = Skip().StringValue;
        return std::make_unique<SymbolExpr>(m_Ctx.GetVar(name), name);
    }

    if (At(TokenType_Int))
        return std::make_unique<ConstIntExpr>(m_Ctx.GetType("u64"), Skip().IntegerValue);

    if (At(TokenType_Float))
        return std::make_unique<ConstFloatExpr>(m_Ctx.GetType("f64"), Skip().FloatValue);

    Error(m_Token.Where, "unhandled token '{}'", m_Token);
}
