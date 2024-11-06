#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

bool LX::OperatorLOr(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    const auto lb = builder.IRBuilder().CreateIsNotNull(lhs.ValueIR);
    const auto rb = builder.IRBuilder().CreateIsNotNull(rhs.ValueIR);
    ref.Type = builder.Ctx().GetIntType(1, false);
    ref.ValueIR = builder.IRBuilder().CreateOr(lb, rb);
    return true;
}

bool LX::OperatorLXOr(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    const auto lb = builder.IRBuilder().CreateIsNotNull(lhs.ValueIR);
    const auto rb = builder.IRBuilder().CreateIsNotNull(rhs.ValueIR);
    ref.Type = builder.Ctx().GetIntType(1, false);
    ref.ValueIR = builder.IRBuilder().CreateXor(lb, rb);
    return true;
}

bool LX::OperatorLAnd(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    const auto lb = builder.IRBuilder().CreateIsNotNull(lhs.ValueIR);
    const auto rb = builder.IRBuilder().CreateIsNotNull(rhs.ValueIR);
    ref.Type = builder.Ctx().GetIntType(1, false);
    ref.ValueIR = builder.IRBuilder().CreateAnd(lb, rb);
    return true;
}

bool LX::OperatorEQ(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = builder.Ctx().GetIntType(1, false);

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateICmpEQ(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFCmpOEQ(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorNE(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = builder.Ctx().GetIntType(1, false);

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateICmpNE(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFCmpONE(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorLT(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = builder.Ctx().GetIntType(1, false);

    if (lhs.Type->IsInt())
    {
        if (lhs.Type->IsSigned())
            ref.ValueIR = builder.IRBuilder().CreateICmpSLT(lhs.ValueIR, rhs.ValueIR);
        else ref.ValueIR = builder.IRBuilder().CreateICmpULT(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFCmpOLT(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorLE(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = builder.Ctx().GetIntType(1, false);

    if (lhs.Type->IsInt())
    {
        if (lhs.Type->IsSigned())
            ref.ValueIR = builder.IRBuilder().CreateICmpSLE(lhs.ValueIR, rhs.ValueIR);
        else ref.ValueIR = builder.IRBuilder().CreateICmpULE(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFCmpOLE(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorGT(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = builder.Ctx().GetIntType(1, false);

    if (lhs.Type->IsInt())
    {
        if (lhs.Type->IsSigned())
            ref.ValueIR = builder.IRBuilder().CreateICmpSGT(lhs.ValueIR, rhs.ValueIR);
        else ref.ValueIR = builder.IRBuilder().CreateICmpUGT(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFCmpOGT(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorGE(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = builder.Ctx().GetIntType(1, false);

    if (lhs.Type->IsInt())
    {
        if (lhs.Type->IsSigned())
            ref.ValueIR = builder.IRBuilder().CreateICmpSGE(lhs.ValueIR, rhs.ValueIR);
        else ref.ValueIR = builder.IRBuilder().CreateICmpUGE(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFCmpOGE(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorAdd(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateAdd(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFAdd(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorSub(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateSub(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFSub(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorMul(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateMul(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFMul(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorDiv(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        if (lhs.Type->IsSigned())
            ref.ValueIR = builder.IRBuilder().CreateSDiv(lhs.ValueIR, rhs.ValueIR);
        else ref.ValueIR = builder.IRBuilder().CreateUDiv(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFDiv(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorRem(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        if (lhs.Type->IsSigned())
            ref.ValueIR = builder.IRBuilder().CreateSRem(lhs.ValueIR, rhs.ValueIR);
        else ref.ValueIR = builder.IRBuilder().CreateURem(lhs.ValueIR, rhs.ValueIR);
        return true;
    }
    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFRem(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorPow(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateBinaryIntrinsic(llvm::Intrinsic::pow, lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorRt(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsFloat())
    {
        const auto r = builder.IRBuilder().CreateFDiv(
            llvm::ConstantFP::get(
                rhs.Type->GetIR(builder),
                1.0),
            rhs.ValueIR);
        ref.ValueIR = builder.IRBuilder().CreateBinaryIntrinsic(llvm::Intrinsic::pow, lhs.ValueIR, r);
        return true;
    }

    return false;
}

bool LX::OperatorOr(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateOr(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorXOr(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateXor(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorAnd(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateAnd(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorShL(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateShl(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorShR(Builder& builder, const Value& lhs, const Value& rhs, Value& ref)
{
    ref.Type = lhs.Type;

    if (lhs.Type->IsInt())
    {
        if (lhs.Type->IsSigned())
            ref.ValueIR = builder.IRBuilder().CreateAShr(lhs.ValueIR, rhs.ValueIR);
        else ref.ValueIR = builder.IRBuilder().CreateLShr(lhs.ValueIR, rhs.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorNeg(Builder& builder, const Value& val, Value& ref)
{
    ref.Type = val.Type;

    if (val.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateNeg(val.ValueIR);
        return true;
    }
    if (val.Type->IsFloat())
    {
        ref.ValueIR = builder.IRBuilder().CreateFNeg(val.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorNot(Builder& builder, const Value& val, Value& ref)
{
    ref.Type = val.Type;

    if (val.Type->IsInt())
    {
        ref.ValueIR = builder.IRBuilder().CreateNot(val.ValueIR);
        return true;
    }

    return false;
}

bool LX::OperatorLNot(Builder& builder, const Value& val, Value& ref)
{
    ref.Type = builder.Ctx().GetIntType(1, false);
    ref.ValueIR = builder.IRBuilder().CreateIsNull(val.ValueIR);
    return true;
}
