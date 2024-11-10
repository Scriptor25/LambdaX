#include <LX/AST.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::ExprPtr LX::Parser::ParseOperand()
{
    auto expr = ParsePrimary();
    while (true)
    {
        if (At("("))
        {
            const auto where = Skip().Where;

            std::vector<ExprPtr> args;
            ParseExprList(args, ")");
            Expect(")");
            const auto type = expr->Type->Element()->Result();
            expr = std::make_unique<CallExpr>(where, type, std::move(expr), std::move(args));
            continue;
        }

        if (At("["))
        {
            const auto where = Skip().Where;

            auto index = ParseExpr();
            Expect("]");
            const auto type = expr->Type->Element();
            expr = std::make_unique<SubscriptExpr>(where, type, std::move(expr), std::move(index));
            continue;
        }

        if (At("as"))
        {
            const auto where = Skip().Where;

            const auto type = ParseType();
            expr = std::make_unique<CastExpr>(where, type, std::move(expr));
            continue;
        }

        if (At(".") || At("!"))
        {
            const auto where = m_Token.Where;

            const auto deref = NextAt("!");
            if (!deref) Expect(".");
            const auto member = Expect(TokenType_Symbol).StringValue;

            TypePtr struct_type = expr->Type;
            if (deref) struct_type = struct_type->Element();

            const auto type = struct_type->Element(struct_type->IndexOf(member));

            expr = std::make_unique<MemberExpr>(where, type, std::move(expr), member, deref);
            continue;
        }

        return expr;
    }
}
