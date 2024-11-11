#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ImmutableExpr::ImmutableExpr(SourceLocation where, std::string name, TypePtr type, ExprPtr init)
    : Expr(std::move(where)), Name(std::move(name)), Type(std::move(type)), Init(std::move(init))
{
}

std::ostream& LX::ImmutableExpr::Print(std::ostream& os) const
{
    os << Name;
    if (Type) Type->Print(os << " => ");
    return Init->Print(os << " := ");
}

LX::ValuePtr LX::ImmutableExpr::GenIR(Builder& builder) const
{
    auto init = Init->GenIR(builder);

    Where.EmitDI(builder);

    const auto type = Type ? Type : init->Type();
    if (Type) init = builder.Cast(Where, init, Type);
    return builder.Define(Where, Name) = RValue::Create(type, init->Load(builder));
}
