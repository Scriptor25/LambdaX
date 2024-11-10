#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::SubscriptExpr::SubscriptExpr(SourceLocation where, TypePtr type, ExprPtr base, ExprPtr offset)
    : Expr(std::move(where), std::move(type)), Base(std::move(base)), Offset(std::move(offset))
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

    const auto gep = builder.IRBuilder().CreateGEP(
        Type->GetIR(builder),
        base->Load(builder),
        {offset->Load(builder)});
    return LValue::Create(Type, gep);
}
