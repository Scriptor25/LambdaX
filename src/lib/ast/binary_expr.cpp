#include <map>
#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::BinaryExpr::BinaryExpr(std::string op, ExprPtr lhs, ExprPtr rhs)
    : Op(std::move(op)), Lhs(std::move(lhs)), Rhs(std::move(rhs))
{
}

std::ostream& LX::BinaryExpr::Print(std::ostream& os) const
{
    return Rhs->Print(Lhs->Print(os) << ' ' << Op << ' ');
}

void LX::BinaryExpr::GenIR(Builder& builder, Value& ref) const
{
    static std::map<std::string, std::function<bool(Builder&, const Value&, const Value&, Value&)>> ops
    {
        // boolean and compare (iN, uN and fN)
        {"||", OperatorLOr},
        {"^^", OperatorLXOr},
        {"&&", OperatorLAnd},
        {"==", OperatorEQ},
        {"!=", OperatorNE},
        {"<", OperatorLT},
        {"<=", OperatorLE},
        {">", OperatorGT},
        {">=", OperatorGE},

        // arithmetic operations (iN, uN and fN)
        {"+", OperatorAdd},
        {"-", OperatorSub},
        {"*", OperatorMul},
        {"/", OperatorDiv},
        {"%", OperatorRem},
        {"**", OperatorPow},
        {"//", OperatorRt},

        // bitwise operations (iN and uN)
        {"|", OperatorOr},
        {"^", OperatorXOr},
        {"&", OperatorAnd},
        {"<<", OperatorShL},
        {">>", OperatorShR},
    };

    Value lhs, rhs;
    Lhs->GenIR(builder, lhs);
    Rhs->GenIR(builder, rhs);

    if (!lhs) Error("lhs is null");
    if (!rhs) Error("rhs is null");

    if (lhs.Type != rhs.Type)
        builder.Equalize(lhs, rhs);

    if (const auto& op = ops[Op]; op && op(builder, lhs, rhs, ref))
        return;

    Error("undefined operator '{} {} {}'", lhs.Type, Op, rhs.Type);
}
