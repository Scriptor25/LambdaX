#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::MutableExpr::MutableExpr(SourceLocation where, std::string name, TypePtr type, ExprPtr init)
    : Expr(std::move(where)), Name(std::move(name)), Type(std::move(type)), Init(std::move(init))
{
}

std::ostream& LX::MutableExpr::Print(std::ostream& os) const
{
    os << "mut " << Name;
    if (Type) Type->Print(os << " => ");
    if (Init) Init->Print(os << " := ");
    return os;
}

LX::ValuePtr LX::MutableExpr::GenIR(Builder& builder) const
{
    auto init = Init ? Init->GenIR(builder) : nullptr;

    Where.EmitDI(builder);

    const auto type = Type ? Type : init->Type();
    const auto ptr = builder.CreateAlloca(type->GenIR(builder), Name);
    if (Init)
    {
        if (Type) init = builder.Cast(Where, init, type);
        builder.IRBuilder().CreateStore(init->Load(builder), ptr);
    }

    return builder.Define(Where, Name) = LValue::Create(type, ptr);
}
