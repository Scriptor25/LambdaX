#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>

LX::TypePtr LX::Parser::ParseType()
{
    TypePtr type;

    if (NextAt("["))
    {
        auto element = m_Ctx.GetVoidType();
        size_t count = 0;

        if (!NextAt("]"))
        {
            element = ParseType();
            if (NextAt(","))
                count = Expect(TokenType_Int).IntegerValue;
            Expect("]");
        }

        if (!count)
            type = m_Ctx.GetPointerType(element);
        else type = m_Ctx.GetArrayType(element, count);
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
    else if (At("struct"))
    {
        const auto where = Skip().Where;

        std::string name;
        if (At(TokenType_String))
            name = Skip().StringValue;

        std::vector<Parameter> elements;
        if (NextAt("{"))
        {
            ParseParameterList(elements, "}");
            Expect("}");
        }

        type = m_Ctx.GetStructType(where, name, elements);
    }
    else
    {
        const auto where = m_Token.Where;
        const auto name = Expect(TokenType_Symbol).StringValue;
        type = m_Ctx.GetType(name);
        if (!type)
            Error(where, "'{}' is not an existing type", name);
    }

    while (NextAt("&"))
        type = m_Ctx.GetReferenceType(type);

    return type;
}
