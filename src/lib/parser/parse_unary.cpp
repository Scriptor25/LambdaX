#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseUnary()
{
    const auto [
        where_,
        type_,
        operator_,
        int_,
        float_
    ] = Skip();

    auto operand = ParseOperand();
    return std::make_unique<UnaryExpr>(where_, operator_, std::move(operand));
}
