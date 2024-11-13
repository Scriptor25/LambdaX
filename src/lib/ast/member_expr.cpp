#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::MemberExpr::MemberExpr(SourceLocation where, ExprPtr parent, std::string member, const bool deref)
    : Expr(std::move(where)), Parent(std::move(parent)), Member(std::move(member)), Deref(deref)
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
        parent = LValue::Create(parent->Type()->Element(Where), parent->Load(Where, builder), parent->IsMutable());

    Where.EmitDI(builder);

    const auto index = parent->Type()->IndexOf(Where, Member);
    const auto type = parent->Type()->Element(Where, index);

    if (parent->HasPtr())
    {
        const auto gep = builder.IRBuilder().CreateStructGEP(
            parent->Type()->GenIR(Where, builder),
            parent->Ptr(Where),
            index,
            Member);
        return LValue::Create(type, gep, parent->IsMutable());
    }

    const auto value = builder.IRBuilder().CreateExtractValue(parent->Load(Where, builder), index, Member);
    return RValue::Create(type, value);
}
