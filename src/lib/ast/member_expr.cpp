#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::MemberExpr::MemberExpr(ExprPtr parent, std::string member)
    : Parent(std::move(parent)), Member(std::move(member))
{
}

std::ostream& LX::MemberExpr::Print(std::ostream& os) const
{
    return Parent->Print(os) << '.' << Member;
}

void LX::MemberExpr::GenIR(Builder& builder, Value& ref) const
{
    Value parent;
    Parent->GenIR(builder, parent);

    const auto index = parent.Type->IndexOf(Member);

    ref.Type = parent.Type->Element(index);
    ref.ValueIR = builder.IRBuilder().CreateExtractValue(parent.ValueIR, index);
}
