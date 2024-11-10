#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParsePrimary()
{
    if (At("("))
    {
        const auto where = Skip().Where;

        m_Ctx.Push();

        std::vector<ExprPtr> children;
        do
        {
            children.push_back(ParseExpr());
            if (!At(")"))
                Expect(",");
        }
        while (!(At(")") || AtEOF()));
        Expect(")");

        m_Ctx.Pop();

        const auto type = children.back()->Type;

        return std::make_unique<SequenceExpr>(where, type, std::move(children));
    }

    if (At("{"))
    {
        const auto where = Skip().Where;

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

        return std::make_unique<ConstStructExpr>(where, type, std::move(elements));
    }

    if (At("$"))
    {
        const auto where = Skip().Where;

        m_Ctx.Push();

        std::vector<Parameter> params;
        bool vararg = false;
        if (NextAt("("))
        {
            vararg = ParseParameterList(params, ")");
            for (const auto& [type_, name_] : params)
                m_Ctx.DefVar(where, name_) = type_;
            Expect(")");
        }

        TypePtr result_type;
        if (NextAt("=>"))
            result_type = ParseType();
        else result_type = m_Ctx.GetVoidType();

        const auto type = m_Ctx.GetFunPtrType(result_type, params, vararg);

        Expect("=");
        auto body = ParseExpr();

        m_Ctx.Pop();

        return std::make_unique<ConstFunctionExpr>(where, type, params, std::move(body));
    }

    if (At("mut"))
    {
        const auto where = Skip().Where;

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

        m_Ctx.DefVar(where, name) = type;

        return std::make_unique<MutableExpr>(where, type, name, std::move(init));
    }

    if (At(TokenType_Operator))
    {
        const auto [
            where_,
            type_,
            operator_,
            int_,
            float_
        ] = Skip();

        auto operand = ParseOperand();

        const auto type = UnaryExpr::GetType(where_, m_Ctx, operator_, operand->Type);

        return std::make_unique<UnaryExpr>(where_, type, operator_, std::move(operand));
    }

    if (At(TokenType_Symbol))
    {
        const auto [
            where_,
            type_,
            name_,
            int_,
            float_
        ] = Skip();

        if (m_Ctx.HasVar(name_))
        {
            const auto type = m_Ctx.GetVar(where_, name_);
            return std::make_unique<SymbolExpr>(where_, type, name_);
        }

        Expect("=");
        auto init = ParseExpr();
        const auto type = init->Type;

        m_Ctx.DefVar(where_, name_) = type;

        return std::make_unique<ImmutableExpr>(where_, type, name_, std::move(init));
    }

    if (At(TokenType_Int))
        return std::make_unique<ConstIntExpr>(m_Token.Where, m_Ctx.GetIntType(64, false), Skip().IntegerValue);

    if (At(TokenType_Float))
        return std::make_unique<ConstFloatExpr>(m_Token.Where, m_Ctx.GetFloatType(64), Skip().FloatValue);

    if (At(TokenType_String))
        return std::make_unique<ConstStringExpr>(
            m_Token.Where,
            m_Ctx.GetPointerType(m_Ctx.GetIntType(8, true)),
            Skip().StringValue);

    Error(m_Token.Where, "unhandled token '{}'", m_Token);
}
