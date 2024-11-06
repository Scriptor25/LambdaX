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
            expr = std::make_unique<CallExpr>(std::move(expr), std::move(args));
            continue;
        }

        if (NextAt("["))
        {
            auto index = ParseExpr();
            Expect("]");
            expr = std::make_unique<SubscriptExpr>(std::move(expr), std::move(index));
            continue;
        }

        if (NextAt("as"))
        {
            const auto type = ParseType();
            expr = std::make_unique<CastExpr>(std::move(expr), type);
            continue;
        }

        if (NextAt("."))
        {
            const auto member = Expect(TokenType_Symbol).StringValue;
            expr = std::make_unique<MemberExpr>(std::move(expr), member);
            continue;
        }

        return expr;
    }
}
