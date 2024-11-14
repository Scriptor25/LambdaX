#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::SubscriptExpr::SubscriptExpr(SourceLocation where, ExprPtr base, ExprPtr offset)
    : Expr(std::move(where)), Base(std::move(base)), Offset(std::move(offset))
{
}

std::ostream& LX::SubscriptExpr::Print(std::ostream& os) const
{
    return Offset->Print(Base->Print(os) << '[') << ']';
}

LX::ValuePtr LX::SubscriptExpr::GenIR(Builder& builder) const
{
    const auto base = Base->GenIR(builder);
    const auto offset = Offset->GenIR(builder);
    const auto type = base->Type()->Base(Where);

    Where.EmitDI(builder);

    if (base->Type()->IsPointer())
    {
        const auto gep = builder.IRBuilder().CreateGEP(
            type->GenIR(Where, builder),
            base->Load(Where, builder),
            {offset->Load(Where, builder)});
        return LValue::Create(type, gep, base->Type()->IsMutable());
    }
    if (base->Type()->IsArray())
    {
        const auto zero = builder.IRBuilder().getInt64(0);
        const auto gep = builder.IRBuilder().CreateGEP(
            base->Type()->GenIR(Where, builder),
            base->Ptr(Where),
            {zero, offset->Load(Where, builder)});
        return LValue::Create(type, gep, base->Type()->IsMutable());
    }

    Error(Where, "base of subscript must be a pointer or array type, but is {}", base->Type());
}
