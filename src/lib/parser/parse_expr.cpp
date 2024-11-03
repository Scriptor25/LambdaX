#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::ExprPtr LX::Parser::ParseExpr()
{
    auto expr = ParseBinary(ParseOperand(), 0);
    while (At("?"))
    {
        const auto where = Skip().Where;
        auto then = ParseExpr();
        Expect(":");
        auto else_ = ParseExpr();
        if (then->Type != else_->Type)
            Error(
                where,
                "ternary operation requires identical branch results, but got '{}' and '{}'",
                then->Type,
                else_->Type);
        expr = std::make_unique<TernaryExpr>(then->Type, std::move(expr), std::move(then), std::move(else_));
    }
    return expr;
}
