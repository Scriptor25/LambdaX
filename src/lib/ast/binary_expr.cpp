#include <map>
#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::TypePtr LX::BinaryExpr::GetType(Context& ctx, const std::string& operator_, const TypePtr& lhs, const TypePtr& rhs)
{
    static std::map<std::string, std::function<TypePtr(Context&, const TypePtr&)>> OPS
    {
        {"||", OperatorTypeLogical},
        {"^^", OperatorTypeLogical},
        {"&&", OperatorTypeLogical},

        {"==", OperatorTypeCmp},
        {"!=", OperatorTypeCmp},
        {"<", OperatorTypeCmp},
        {"<=", OperatorTypeCmp},
        {">", OperatorTypeCmp},
        {">=", OperatorTypeCmp},

        {"+", OperatorTypeArith},
        {"-", OperatorTypeArith},
        {"*", OperatorTypeArith},
        {"/", OperatorTypeArith},
        {"%", OperatorTypeArith},

        {"**", OperatorTypePow},
        {"//", OperatorTypePow},

        {"|", OperatorTypeBitwise},
        {"^", OperatorTypeBitwise},
        {"&", OperatorTypeBitwise},
        {"<<", OperatorTypeBitwise},
        {">>", OperatorTypeBitwise},
    };

    if (operator_ == "=")
        return lhs;

    const auto equ = Type::Equalize(ctx, lhs, rhs);

    if (const auto& op = OPS[operator_]; op)
        if (const auto type = op(ctx, equ))
            return type;

    Error("undefined binary operator '{} {} {}'", lhs, operator_, rhs);
}

LX::BinaryExpr::BinaryExpr(TypePtr type, std::string op, ExprPtr lhs, ExprPtr rhs)
    : Expr(std::move(type)), Op(std::move(op)), Lhs(std::move(lhs)), Rhs(std::move(rhs))
{
}

std::ostream& LX::BinaryExpr::Print(std::ostream& os) const
{
    return Rhs->Print(Lhs->Print(os) << ' ' << Op << ' ');
}

LX::ValuePtr LX::BinaryExpr::GenIR(Builder& builder) const
{
    static std::map<std::string, std::function<ValuePtr(Builder&, const ValuePtr&, const ValuePtr&)>> OPS
    {
        {"||", OperatorLOr},
        {"^^", OperatorLXOr},
        {"&&", OperatorLAnd},

        {"==", OperatorEQ},
        {"!=", OperatorNE},
        {"<", OperatorLT},
        {"<=", OperatorLE},
        {">", OperatorGT},
        {">=", OperatorGE},

        {"+", OperatorAdd},
        {"-", OperatorSub},
        {"*", OperatorMul},
        {"/", OperatorDiv},
        {"%", OperatorRem},

        {"**", OperatorPow},
        {"//", OperatorRt},

        {"|", OperatorOr},
        {"^", OperatorXOr},
        {"&", OperatorAnd},
        {"<<", OperatorShL},
        {">>", OperatorShR},
    };

    auto lhs = Lhs->GenIR(builder);
    auto rhs = Rhs->GenIR(builder);

    if (Op == "=")
    {
        lhs->Store(builder, rhs->Load(builder));
        return lhs;
    }

    builder.Equalize(lhs, rhs);

    if (const auto& op = OPS[Op]; op)
        if (const auto value = op(builder, lhs, rhs))
            return value;

    Error("undefined binary operator '{} {} {}'", lhs->Type(), Op, rhs->Type());
}
