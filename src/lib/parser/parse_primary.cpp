#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParsePrimary()
{
    if (NextAt("("))
    {
        std::vector<ExprPtr> children;
        do
        {
            children.push_back(ParseExpr());
            if (!At(")"))
                Expect(",");
        }
        while (!(At(")") || AtEOF()));
        Expect(")");
        return std::make_unique<SequenceExpr>(std::move(children));
    }

    if (NextAt("$"))
    {
        std::vector<Parameter> params;
        if (NextAt("("))
        {
            ParseParameterList(params, ")");
            Expect(")");
        }
        Expect("=>");
        const auto result_type = ParseType();
        std::vector<TypePtr> param_types;
        for (const auto& [type, name] : params)
            param_types.push_back(type);
        const auto type = m_Ctx.GetFunctionType(result_type, param_types);
        Expect("=");
        auto body = ParseExpr();
        return std::make_unique<ConstFunctionExpr>(type, params, std::move(body));
    }

    if (At(TokenType_Symbol))
    {
        const auto name = Skip().StringValue;
        return std::make_unique<SymbolExpr>(name);
    }

    if (At(TokenType_Int))
        return std::make_unique<ConstIntExpr>(Skip().IntegerValue);

    if (At(TokenType_Float))
        return std::make_unique<ConstFloatExpr>(Skip().FloatValue);

    if (At(TokenType_String))
        return std::make_unique<ConstStringExpr>(Skip().StringValue);

    Error(m_Token.Where, "unhandled token '{}'", m_Token);
}
