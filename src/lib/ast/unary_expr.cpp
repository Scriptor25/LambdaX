#include <map>
#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::TypePtr LX::UnaryExpr::GetType(
    const SourceLocation& where,
    Context& ctx,
    const std::string& operator_,
    const TypePtr& operand)
{
    static std::map<std::string, std::function<TypePtr(const SourceLocation&, Context&, const TypePtr&)>> OPS
    {
        {"-", {OperatorTypeNeg}},
        {"~", {OperatorTypeNot}},
        {"!", {OperatorTypeLNot}},
        {"&", {OperatorTypeRef}},
        {"*", {OperatorTypeDeref}},
    };

    if (const auto& op = OPS[operator_]; op)
        if (const auto type = op(where, ctx, operand))
            return type;

    Error(where, "undefined unary operator '{}{}'", operator_, operand);
}

LX::UnaryExpr::UnaryExpr(SourceLocation where, std::string operator_, ExprPtr operand)
    : Expr(std::move(where)), Operator(std::move(operator_)), Operand(std::move(operand))
{
}

std::ostream& LX::UnaryExpr::Print(std::ostream& os) const
{
    return Operand->Print(os << Operator);
}

LX::ValuePtr LX::UnaryExpr::GenIR(Builder& builder) const
{
    static std::map<std::string, std::function<ValuePtr(const SourceLocation&, Builder&, const ValuePtr&)>> OPS
    {
        {"-", {OperatorNeg}},
        {"~", {OperatorNot}},
        {"!", {OperatorLNot}},
        {"&", {OperatorRef}},
        {"*", {OperatorDeref}},
    };

    const auto operand = Operand->GenIR(builder);

    Where.EmitDI(builder);

    if (const auto& op = OPS[Operator]; op)
        if (const auto value = op(Where, builder, operand))
            return value;

    Error(Where, "undefined unary operator '{}{}'", Operator, operand->Type());
}
