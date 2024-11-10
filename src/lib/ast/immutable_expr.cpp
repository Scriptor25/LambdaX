#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

LX::ImmutableExpr::ImmutableExpr(SourceLocation where, TypePtr type, std::string name, ExprPtr init)
    : Expr(std::move(where), std::move(type)), Name(std::move(name)), Init(std::move(init))
{
}

std::ostream& LX::ImmutableExpr::Print(std::ostream& os) const
{
    return Init->Print(Type->Print(os << Name << " => ") << " = ");
}

LX::ValuePtr LX::ImmutableExpr::GenIR(Builder& builder) const
{
    const auto init = Init->GenIR(builder);
    return builder.DefVar(Where, Name) = RValue::Create(Type, init->Load(builder));
}
