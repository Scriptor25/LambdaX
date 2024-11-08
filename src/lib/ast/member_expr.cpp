#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::MemberExpr::MemberExpr(TypePtr type, ExprPtr parent, std::string member, const bool deref)
    : Expr(std::move(type)), Parent(std::move(parent)), Member(std::move(member)), Deref(deref)
{
}

std::ostream& LX::MemberExpr::Print(std::ostream& os) const
{
    return Parent->Print(os) << (Deref ? '!' : '.') << Member;
}

LX::ValuePtr LX::MemberExpr::GenIR(Builder& builder) const
{
    auto parent = Parent->GenIR(builder);
    if (Deref)
        parent = LValue::Create(parent->Type()->Element(), parent->Load(builder));

    const auto index = parent->Type()->IndexOf(Member);

    if (parent->IsMutable())
    {
        const auto gep = builder.IRBuilder().CreateStructGEP(
            parent->Type()->GetIR(builder),
            parent->Ptr(),
            index,
            Member);
        return LValue::Create(Type, gep);
    }

    const auto value = builder.IRBuilder().CreateExtractValue(parent->Load(builder), index, Member);
    return RValue::Create(Type, value);
}
