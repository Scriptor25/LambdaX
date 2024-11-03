#include <LX/Builder.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

bool LX::OperatorLOr(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorLXOr(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorLAnd(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorEQ(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorNE(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorLT(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorLE(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;
    ref.TypeIR = lhs.TypeIR;

    if (lhs.Type->IsInt())
    {
        if (lhs.Type->IsSigned())
            ref.ValueIR = builder.IRBuilder().CreateICmpSLE(lhs.ValueIR, rhs.ValueIR);
        else ref.ValueIR = builder.IRBuilder().CreateICmpULE(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorGT(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorGE(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorAdd(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;
    ref.TypeIR = lhs.TypeIR;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateAdd(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorSub(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;
    ref.TypeIR = lhs.TypeIR;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateSub(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorMul(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;
    ref.TypeIR = lhs.TypeIR;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateMul(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorDiv(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorRem(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorPow(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorRt(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorOr(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorXOr(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorAnd(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorShL(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}

bool LX::OperatorShR(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    return false;
}
