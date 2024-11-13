#include <map>
#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::TypePtr LX::BinaryExpr::GetType(
    const SourceLocation& where,
    Context& ctx,
    const std::string& operator_,
    const TypePtr& left,
    const TypePtr& right)
{
    static std::map<std::string, std::function<TypePtr(const SourceLocation&, Context&, const TypePtr&)>> OPS
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
        return left;

    const auto equ = Type::Equalize(where, ctx, left, right);

    if (const auto& op = OPS[operator_]; op)
        if (const auto type = op(where, ctx, equ))
            return type;

    Error(where, "undefined binary operator '{} {} {}'", left, operator_, right);
}

LX::BinaryExpr::BinaryExpr(SourceLocation where, std::string operator_, ExprPtr left, ExprPtr right)
    : Expr(std::move(where)),
      Operator(std::move(operator_)),
      Left(std::move(left)),
      Right(std::move(right))
{
}

std::ostream& LX::BinaryExpr::Print(std::ostream& os) const
{
    return Right->Print(Left->Print(os) << ' ' << Operator << ' ');
}

LX::ValuePtr LX::BinaryExpr::GenIR(Builder& builder) const
{
    static std::map<std::string, std::function<ValuePtr(const SourceLocation&, Builder&, const ValuePtr&,
                                                        const ValuePtr&)>> OPS
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

    auto lhs = Left->GenIR(builder);
    auto rhs = Right->GenIR(builder);

    Where.EmitDI(builder);

    if (Operator == "=")
    {
        rhs = builder.CreateCast(Where, rhs, lhs->Type());
        lhs->Store(Where, builder, rhs->Load(Where, builder));
        return lhs;
    }

    builder.Equalize(Where, lhs, rhs);

    if (const auto& op = OPS[Operator]; op)
        if (const auto value = op(Where, builder, lhs, rhs))
            return value;

    Error(Where, "undefined binary operator '{} {} {}'", lhs->Type(), Operator, rhs->Type());
}
