#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Operator.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ValuePtr LX::OperatorLOr(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto lb = builder.IRBuilder().CreateIsNotNull(lhs->Load(where, builder));
    const auto rb = builder.IRBuilder().CreateIsNotNull(rhs->Load(where, builder));
    const auto type = builder.Ctx().GetIntType(1, false);
    const auto value = builder.IRBuilder().CreateOr(lb, rb);
    return RValue::Create(type, value);
}

LX::ValuePtr LX::OperatorLXOr(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto lb = builder.IRBuilder().CreateIsNotNull(lhs->Load(where, builder));
    const auto rb = builder.IRBuilder().CreateIsNotNull(rhs->Load(where, builder));
    const auto type = builder.Ctx().GetIntType(1, false);
    const auto value = builder.IRBuilder().CreateXor(lb, rb);
    return RValue::Create(type, value);
}

LX::ValuePtr LX::OperatorLAnd(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto lb = builder.IRBuilder().CreateIsNotNull(lhs->Load(where, builder));
    const auto rb = builder.IRBuilder().CreateIsNotNull(rhs->Load(where, builder));
    const auto type = builder.Ctx().GetIntType(1, false);
    const auto value = builder.IRBuilder().CreateAnd(lb, rb);
    return RValue::Create(type, value);
}

LX::TypePtr LX::OperatorTypeLogical(const SourceLocation& where, Context& ctx, const TypePtr&)
{
    return ctx.GetIntType(1, false);
}

LX::ValuePtr LX::OperatorEQ(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateICmpEQ(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOEQ(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorNE(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateICmpNE(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpONE(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorLT(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateICmpSLT(lhs->Load(where, builder), rhs->Load(where, builder));
        else value = builder.IRBuilder().CreateICmpULT(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOLT(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorLE(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateICmpSLE(lhs->Load(where, builder), rhs->Load(where, builder));
        else value = builder.IRBuilder().CreateICmpULE(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOLE(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorGT(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateICmpSGT(lhs->Load(where, builder), rhs->Load(where, builder));
        else value = builder.IRBuilder().CreateICmpUGT(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOGT(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorGE(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = builder.Ctx().GetIntType(1, false);

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateICmpSGE(lhs->Load(where, builder), rhs->Load(where, builder));
        else value = builder.IRBuilder().CreateICmpUGE(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFCmpOGE(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeCmp(const SourceLocation&, Context& ctx, const TypePtr& type)
{
    if (type->IsInt() || type->IsFloat())
        return ctx.GetIntType(1, false);
    return {};
}

LX::ValuePtr LX::OperatorAdd(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateAdd(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFAdd(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorSub(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateSub(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFSub(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorMul(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateMul(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFMul(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorDiv(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateSDiv(lhs->Load(where, builder), rhs->Load(where, builder));
        else value = builder.IRBuilder().CreateUDiv(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFDiv(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorRem(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateSRem(lhs->Load(where, builder), rhs->Load(where, builder));
        else value = builder.IRBuilder().CreateURem(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFRem(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeArith(const SourceLocation&, Context&, const TypePtr& type)
{
    if (type->IsInt() || type->IsFloat())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorPow(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateBinaryIntrinsic(
            llvm::Intrinsic::pow,
            lhs->Load(where, builder),
            rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorRt(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsFloat())
    {
        const auto r = builder.IRBuilder().CreateFDiv(
            llvm::ConstantFP::get(
                rhs->Type()->GenIR(where, builder),
                1.0),
            rhs->Load(where, builder));
        const auto value = builder.IRBuilder().CreateBinaryIntrinsic(
            llvm::Intrinsic::pow,
            lhs->Load(where, builder),
            r);
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypePow(const SourceLocation&, Context&, const TypePtr& type)
{
    if (type->IsFloat())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorOr(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateOr(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorXOr(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateXor(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorAnd(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateAnd(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorShL(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateShl(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::ValuePtr LX::OperatorShR(const SourceLocation& where, Builder& builder, const ValuePtr& lhs, const ValuePtr& rhs)
{
    const auto type = lhs->Type();

    if (lhs->Type()->IsInt())
    {
        llvm::Value* value;
        if (lhs->Type()->IsSigned())
            value = builder.IRBuilder().CreateAShr(lhs->Load(where, builder), rhs->Load(where, builder));
        else value = builder.IRBuilder().CreateLShr(lhs->Load(where, builder), rhs->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeBitwise(const SourceLocation&, Context&, const TypePtr& type)
{
    if (type->IsInt())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorNeg(const SourceLocation& where, Builder& builder, const ValuePtr& val)
{
    const auto type = val->Type();

    if (val->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateNeg(val->Load(where, builder));
        return RValue::Create(type, value);
    }
    if (val->Type()->IsFloat())
    {
        const auto value = builder.IRBuilder().CreateFNeg(val->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeNeg(const SourceLocation&, Context&, const TypePtr& type)
{
    if (type->IsInt() || type->IsFloat())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorNot(const SourceLocation& where, Builder& builder, const ValuePtr& val)
{
    const auto type = val->Type();

    if (val->Type()->IsInt())
    {
        const auto value = builder.IRBuilder().CreateNot(val->Load(where, builder));
        return RValue::Create(type, value);
    }

    return {};
}

LX::TypePtr LX::OperatorTypeNot(const SourceLocation&, Context&, const TypePtr& type)
{
    if (type->IsInt())
        return type;
    return {};
}

LX::ValuePtr LX::OperatorLNot(const SourceLocation& where, Builder& builder, const ValuePtr& val)
{
    const auto type = builder.Ctx().GetIntType(1, false);
    const auto value = builder.IRBuilder().CreateIsNull(val->Load(where, builder));
    return RValue::Create(type, value);
}

LX::TypePtr LX::OperatorTypeLNot(const SourceLocation&, Context& ctx, const TypePtr&)
{
    return ctx.GetIntType(1, false);
}

LX::ValuePtr LX::OperatorRef(const SourceLocation& where, Builder& builder, const ValuePtr& val)
{
    return RValue::Create(builder.Ctx().GetPointerType(val->IsMutable(), val->Type()), val->Ptr(where));
}

LX::TypePtr LX::OperatorTypeRef(const SourceLocation&, Context& ctx, const TypePtr& type)
{
    return ctx.GetPointerType(false, type);
}

LX::ValuePtr LX::OperatorDeref(const SourceLocation& where, Builder& builder, const ValuePtr& val)
{
    if (val->Type()->IsPointer())
        return LValue::Create(val->Type()->Base(where), val->Load(where, builder), val->IsMutable());
    return {};
}

LX::TypePtr LX::OperatorTypeDeref(const SourceLocation& where, Context&, const TypePtr& type)
{
    if (type->IsPointer())
        return type->Base(where);
    return {};
}
