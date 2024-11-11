#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ValuePtr LX::OperatorLOr(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto lb = builder.IRBuilder().CreateIsNotNull(lhs->Load(builder));
    const auto rb = builder.IRBuilder().CreateIsNotNull(rhs->Load(builder));
    const auto type = builder.Ctx().GetIntType(1, false);
    const auto value = builder.IRBuilder().CreateOr(lb, rb);
    return RValue::Create(type, value);
}

LX::ValuePtr LX::OperatorLXOr(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto lb = builder.IRBuilder().CreateIsNotNull(lhs->Load(builder));
    const auto rb = builder.IRBuilder().CreateIsNotNull(rhs->Load(builder));
    const auto type = builder.Ctx().GetIntType(1, false);
    const auto value = builder.IRBuilder().CreateXor(lb, rb);
    return RValue::Create(type, value);
}

LX::ValuePtr LX::OperatorLAnd(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto lb = builder.IRBuilder().CreateIsNotNull(lhs->Load(builder));
    const auto rb = builder.IRBuilder().CreateIsNotNull(rhs->Load(builder));
    const auto type = builder.Ctx().GetIntType(1, false);
    const auto value = builder.IRBuilder().CreateAnd(lb, rb);
    return RValue::Create(type, value);
}

LX::TypePtr LX::OperatorTypeLogical(Context& ctx, const TypePtr&)
{
    return ctx.GetIntType(1, false);
}

LX::ValuePtr LX::OperatorEQ(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateICmpEQ(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOEQ(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorNE(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateICmpNE(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpONE(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorLT(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateICmpSLT(lhs->Load(builder), rhs->Load(builder));
        else value = builder.IRBuilder().CreateICmpULT(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOLT(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorLE(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateICmpSLE(lhs->Load(builder), rhs->Load(builder));
        else value = builder.IRBuilder().CreateICmpULE(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOLE(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorGT(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateICmpSGT(lhs->Load(builder), rhs->Load(builder));
        else value = builder.IRBuilder().CreateICmpUGT(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOGT(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorGE(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateICmpSGE(lhs->Load(builder), rhs->Load(builder));
        else value = builder.IRBuilder().CreateICmpUGE(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOGE(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeCmp(Context& ctx, const TypePtr& type)
{
    if (type->IsInt() || type->IsFloat())
        return ctx.GetIntType(1, false);
    return {};
}

LX::ValuePtr LX::OperatorAdd(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateAdd(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFAdd(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorSub(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateSub(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFSub(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorMul(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateMul(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFMul(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorDiv(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateSDiv(lhs->Load(builder), rhs->Load(builder));
        else value = builder.IRBuilder().CreateUDiv(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFDiv(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorRem(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateSRem(lhs->Load(builder), rhs->Load(builder));
        else value = builder.IRBuilder().CreateURem(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFRem(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeArith(Context&, const TypePtr& type)
{
    if (type->IsInt() || type->IsFloat())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorPow(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().
                                   CreateBinaryIntrinsic(llvm::Intrinsic::pow, lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorRt(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsFloat())
    {
        const auto r = builder.IRBuilder().CreateFDiv(
            llvm::ConstantFP::get(
                rhs->Type()->GenIR(builder),
                1.0),
            rhs->Load(builder));
        const auto value = builder.IRBuilder().CreateBinaryIntrinsic(llvm::Intrinsic::pow, lhs->Load(builder), r);
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypePow(Context&, const TypePtr& type)
{
    if (type->IsFloat())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorOr(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateOr(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorXOr(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateXor(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorAnd(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateAnd(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorShL(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateShl(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorShR(Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateAShr(lhs->Load(builder), rhs->Load(builder));
        else value = builder.IRBuilder().CreateLShr(lhs->Load(builder), rhs->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeBitwise(Context&, const TypePtr& type)
{
    if (type->IsInt())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorNeg(Builder& builder, const ValuePtr& val)
{
    const auto type = val->Type();

    if (val->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateNeg(val->Load(builder));
        return RValue::Create(type, value);
    }
    if (val->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFNeg(val->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeNeg(Context& ctx, const TypePtr& type)
{
    if (type->IsInt() || type->IsFloat())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorNot(Builder& builder, const ValuePtr& val)
{
    const auto type = val->Type();

    if (val->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateNot(val->Load(builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeNot(Context& ctx, const TypePtr& type)
{
    if (type->IsInt())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorLNot(Builder& builder, const ValuePtr& val)
{
    const auto type = builder.Ctx().GetIntType(1, false);
    const auto value = builder.IRBuilder().CreateIsNull(val->Load(builder));
    return RValue::Create(type, value);
}

LX::TypePtr LX::OperatorTypeLNot(Context& ctx, const TypePtr&)
{
    return ctx.GetIntType(1, false);
}

LX::ValuePtr LX::OperatorRef(Builder& builder, const ValuePtr& val)
{
    return RValue::Create(builder.Ctx().GetPointerType(val->Type()), val->Ptr());
}

LX::TypePtr LX::OperatorTypeRef(Context& ctx, const TypePtr& type)
{
    return ctx.GetPointerType(type);
}

LX::ValuePtr LX::OperatorDeref(Builder& builder, const ValuePtr& val)
{
    if (val->Type()->IsPointer())
        return LValue::Create(val->Type()->Element(), val->Load(builder));
    return {};
}

LX::TypePtr LX::OperatorTypeDeref(Context& ctx, const TypePtr& type)
{
    if (type->IsPointer())
        return type->Element();
    return {};
}
