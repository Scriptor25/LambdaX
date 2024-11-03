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
            expr = std::make_unique<CallExpr>(expr->Type->Element()->Result(), std::move(expr), std::move(args));
            continue;
        }

        return expr;
    }
}
