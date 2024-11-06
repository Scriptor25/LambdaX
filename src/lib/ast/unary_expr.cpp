#include <map>
#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::UnaryExpr::UnaryExpr(std::string op, ExprPtr operand)
    : Operator(std::move(op)), Operand(std::move(operand))
{
}

std::ostream& LX::UnaryExpr::Print(std::ostream& os) const
{
    return Operand->Print(os << Operator);
}

void LX::UnaryExpr::GenIR(Builder& builder, Value& ref) const
{
    static std::map<std::string, std::function<bool(Builder&, const Value&, Value&)>> OPS
    {
        {"-", {OperatorNeg}},
        {"~", {OperatorNot}},
        {"!", {OperatorLNot}},
    };

    Value operand;
    Operand->GenIR(builder, operand);

    if (const auto& op = OPS[Operator]; op && op(builder, operand, ref))
        return;

    Error("undefined unary operator '{}{}'", Operator, operand.Type);
}
