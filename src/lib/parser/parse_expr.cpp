#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::ExprPtr LX::Parser::ParseExpr()
{
    auto expr = ParseBinary(ParseOperand(), 0);
    while (NextAt("?"))
    {
        auto then = ParseExpr();
        Expect(":");
        auto else_ = ParseExpr();
        expr = std::make_unique<TernaryExpr>(std::move(expr), std::move(then), std::move(else_));
    }
    return expr;
}
