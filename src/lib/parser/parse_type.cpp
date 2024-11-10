#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>

LX::TypePtr LX::Parser::ParseType()
{
    TypePtr type;

    if (NextAt("["))
    {
        TypePtr element;
        if (!NextAt("]"))
        {
            element = ParseType();
            Expect("]");
        }
        else element = m_Ctx.GetVoidType();
        type = m_Ctx.GetPointerType(element);
    }
    else if (NextAt("("))
    {
        std::vector<Parameter> params;
        const auto vararg = ParseParameterList(params, ")");
        Expect(")");
        TypePtr result;
        if (NextAt("=>"))
            result = ParseType();
        else result = m_Ctx.GetVoidType();
        type = m_Ctx.GetFunctionType(result, params, vararg);
    }
    else if (NextAt("{"))
    {
        std::vector<Parameter> elements;
        ParseParameterList(elements, "}");
        Expect("}");
        type = m_Ctx.GetStructType(elements);
    }
    else if (NextAt("mut"))
    {
        const auto element = ParseType();
        type = m_Ctx.GetMutableType(element);
    }
    else
    {
        const auto where = m_Token.Where;
        const auto name = Expect(TokenType_Symbol).StringValue;
        type = m_Ctx.GetType(name);
        if (!type)
            Error(where, "'{}' is not an existing type", name);
    }

    while (NextAt("["))
    {
        const auto size = Expect(TokenType_Int).IntegerValue;
        Expect("]");
        type = m_Ctx.GetArrayType(type, size);
    }

    return type;
}
