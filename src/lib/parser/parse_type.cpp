#include <LX/Error.hpp>
#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>

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
        const auto vararg = ParseTypeList(params, ")");
        Expect(")");
        Expect("=>");
        const auto result = ParseType();
        return m_Ctx.GetFunctionType(result, params, vararg);
    }

    if (NextAt("{"))
    {
        std::vector<Parameter> elements;
        ParseParameterList(elements, "}");
        Expect("}");
        return m_Ctx.GetStructType(elements);
    }

    const auto name = Expect(TokenType_Symbol).StringValue;
    return m_Ctx.GetType(name);
}
