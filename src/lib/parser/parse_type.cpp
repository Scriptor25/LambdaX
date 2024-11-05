#include <LX/Parser.hpp>

#include "LX/Error.hpp"

LX::TypePtr& LX::Parser::ParseType()
{
    if (NextAt("["))
    {
        TypePtr base;
        if (!NextAt("]"))
        {
            base = ParseType();
            Expect("]");
        }
        return m_Ctx.GetPointerType(base);
    }

    if (NextAt("("))
    {
        std::vector<TypePtr> params;
        ParseTypeList(params, ")");
        Expect(")");
        Expect("=>");
        const auto result = ParseType();
        return m_Ctx.GetFunctionType(result, params);
    }

    const auto name = Expect(TokenType_Symbol).StringValue;
    return m_Ctx.GetType(name);
}
