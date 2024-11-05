#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::SubscriptExpr::SubscriptExpr(ExprPtr base, ExprPtr offset)
    : Base(std::move(base)), Offset(std::move(offset))
{
}

std::ostream& LX::SubscriptExpr::Print(std::ostream& os) const
{
    return Offset->Print(Base->Print(os) << '[') << ']';
}

void LX::SubscriptExpr::GenIR(Builder& builder, Value& ref) const
{
    Value base;
    Base->GenIR(builder, base);
    if (!base) Error("base is null");

    Value offset;
    Offset->GenIR(builder, offset);
    if (!offset) Error("offset is null");

    const auto element_ty = base.Type->Element();
    const auto ptr = builder.IRBuilder().CreateGEP(element_ty->GenIR(builder), base.ValueIR, {offset.ValueIR});
    ref.Type = element_ty;
    ref.ValueIR = builder.IRBuilder().CreateLoad(ref.Type->GenIR(builder), ptr);
}
