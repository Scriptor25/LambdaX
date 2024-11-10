#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

LX::MutableExpr::MutableExpr(SourceLocation where, TypePtr type, std::string name, ExprPtr init)
    : Expr(std::move(where), std::move(type)), Name(std::move(name)), Init(std::move(init))
{
}

std::ostream& LX::MutableExpr::Print(std::ostream& os) const
{
    os << "mut " << Name;
    if (Type) Type->Print(os << " => ");
    if (Init) Init->Print(os << " = ");
    return os;
}

LX::ValuePtr LX::MutableExpr::GenIR(Builder& builder) const
{
    auto init = Init ? Init->GenIR(builder) : nullptr;

    const auto ptr = builder.CreateAlloca(Type->GetIR(builder), Name);
    if (init)
    {
        init = builder.Cast(Where, init, Type);
        builder.IRBuilder().CreateStore(init->Load(builder), ptr);
    }

    return builder.DefVar(Where, Name) = LValue::Create(Type, ptr);
}
