#include <LX/Context.hpp>
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
        std::vector<Parameter> params;
        const auto vararg = ParseParameterList(params, ")");
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

    if (NextAt("mut"))
    {
        const auto type = ParseType();
        return m_Ctx.GetMutableType(type);
    }

    const auto where = m_Token.Where;
    const auto name = Expect(TokenType_Symbol).StringValue;
    if (auto& type = m_Ctx.GetType(name))
        return type;

    Error(where, "'{}' is not an existing type", name);
}
