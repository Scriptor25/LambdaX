#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParsePrimary()
{
    if (NextAt("("))
    {
        m_Ctx.Push();
        std::vector<ExprPtr> children;
        do
        {
            children.push_back(ParseExpr());
            if (!At(")"))
                Expect(",");
        }
        while (!(At(")") || AtEOF()));
        m_Ctx.Pop();
        Expect(")");
        const auto type = children.back()->Type;
        return std::make_unique<SequenceExpr>(type, std::move(children));
    }

    if (NextAt("{"))
    {
        std::vector<ExprPtr> elements;
        while (!(At("}") || AtEOF()))
        {
            elements.push_back(ParseExpr());
            if (!At("}"))
                Expect(",");
        }
        Expect("}");

        TypePtr type;
        if (NextAt("=>"))
            type = ParseType();
        else
        {
            std::vector<Parameter> params(elements.size());
            for (size_t i = 0; i < elements.size(); ++i)
            {
                auto& [type_, name_] = params[i];
                type_ = elements[i]->Type;
            }
            type = m_Ctx.GetStructType(params);
        }

        return std::make_unique<ConstStructExpr>(type, std::move(elements));
    }

    if (NextAt("$"))
    {
        m_Ctx.Push();

        std::vector<Parameter> params;
        bool vararg = false;
        if (NextAt("("))
        {
            vararg = ParseParameterList(params, ")");
            for (const auto& [type_, name_] : params)
                m_Ctx.DefVar(name_) = type_;
            Expect(")");
        }

        Expect("=>");
        const auto result_type = ParseType();

        const auto type = m_Ctx.GetFunPtrType(result_type, params, vararg);

        Expect("=");
        auto body = ParseExpr();

        m_Ctx.Pop();

        return std::make_unique<ConstFunctionExpr>(type, params, std::move(body));
    }

    if (NextAt("mut"))
    {
        const auto name = Expect(TokenType_Symbol).StringValue;

        TypePtr type;
        ExprPtr init;

        if (NextAt("=>"))
            type = ParseType();
        if ((!type && (Expect("="), true)) || NextAt("="))
        {
            init = ParseExpr();
            if (!type) type = init->Type;
        }

        m_Ctx.DefVar(name) = type;

        return std::make_unique<MutableExpr>(name, type, std::move(init));
    }

    if (At(TokenType_Operator))
    {
        const auto op = Skip().StringValue;
        auto operand = ParseOperand();
        const auto type = UnaryExpr::GetType(m_Ctx, op, operand->Type);
        return std::make_unique<UnaryExpr>(type, op, std::move(operand));
    }

    if (At(TokenType_Symbol))
    {
        const auto name = Skip().StringValue;
        if (m_Ctx.HasVar(name))
        {
            const auto type = m_Ctx.GetVar(name);
            return std::make_unique<SymbolExpr>(type, name);
        }

        Expect("=");
        auto init = ParseExpr();
        const auto type = init->Type;

        m_Ctx.DefVar(name) = type;

        return std::make_unique<ImmutableExpr>(name, type, std::move(init));
    }

    if (At(TokenType_Int))
        return std::make_unique<ConstIntExpr>(m_Ctx.GetIntType(64, false), Skip().IntegerValue);

    if (At(TokenType_Float))
        return std::make_unique<ConstFloatExpr>(m_Ctx.GetFloatType(64), Skip().FloatValue);

    if (At(TokenType_String))
        return std::make_unique<ConstStringExpr>(m_Ctx.GetPointerType(m_Ctx.GetIntType(8, true)), Skip().StringValue);

    Error(m_Token.Where, "unhandled token '{}'", m_Token);
}
