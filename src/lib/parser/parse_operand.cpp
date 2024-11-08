#include <LX/AST.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::ExprPtr LX::Parser::ParseOperand()
{
    auto expr = ParsePrimary();
    while (true)
    {
        if (NextAt("("))
        {
            std::vector<ExprPtr> args;
            ParseExprList(args, ")");
            Expect(")");
            const auto type = expr->Type->Element()->Result();
            expr = std::make_unique<CallExpr>(type, std::move(expr), std::move(args));
            continue;
        }

        if (NextAt("["))
        {
            auto index = ParseExpr();
            Expect("]");
            const auto type = expr->Type->Element();
            expr = std::make_unique<SubscriptExpr>(type, std::move(expr), std::move(index));
            continue;
        }

        if (NextAt("as"))
        {
            const auto type = ParseType();
            expr = std::make_unique<CastExpr>(type, std::move(expr));
            continue;
        }

        if (At(".") || At("!"))
        {
            const auto deref = NextAt("!");
            if (!deref) Expect(".");
            const auto member = Expect(TokenType_Symbol).StringValue;
            TypePtr struct_type = expr->Type;
            if (deref) struct_type = struct_type->Element();
            const auto type = struct_type->Element(struct_type->IndexOf(member));
            expr = std::make_unique<MemberExpr>(type, std::move(expr), member, deref);
            continue;
        }

        return expr;
    }
}
