#include <map>
#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::TypePtr LX::UnaryExpr::GetType(Context& ctx, const std::string& operator_, const TypePtr& operand)
{
    static std::map<std::string, std::function<TypePtr(Context&, const TypePtr&)>> OPS
    {
        {"-", {OperatorTypeNeg}},
        {"~", {OperatorTypeNot}},
        {"!", {OperatorTypeLNot}},
    };

    if (const auto& op = OPS[operator_]; op)
        if (const auto type = op(ctx, operand))
            return type;

    Error("undefined unary operator '{}{}'", operator_, operand);
}

LX::UnaryExpr::UnaryExpr(TypePtr type, std::string op, ExprPtr operand)
    : Expr(std::move(type)), Operator(std::move(op)), Operand(std::move(operand))
{
}

std::ostream& LX::UnaryExpr::Print(std::ostream& os) const
{
    return Operand->Print(os << Operator);
}

LX::ValuePtr LX::UnaryExpr::GenIR(Builder& builder) const
{
    static std::map<std::string, std::function<ValuePtr(Builder&, const ValuePtr&)>> OPS
    {
        {"-", {OperatorNeg}},
        {"~", {OperatorNot}},
        {"!", {OperatorLNot}},
    };

    const auto operand = Operand->GenIR(builder);

    if (const auto& op = OPS[Operator]; op)
        if (const auto value = op(builder, operand))
            return value;

    Error("undefined unary operator '{}{}'", Operator, operand->Type());
}
