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
    const auto value = builder.CreateAlloca(Where, type, false, Name);

    if (Type) init = builder.CreateCast(Where, init, Type);
    value->StoreForce(Where, builder, init->Load(Where, builder));

    const auto d = builder.DIBuilder().createAutoVariable(
        &builder.DIScope(),
        Name,
        builder.DIScope().getFile(),
        Where.Row,
        type->GenDI(builder),
        true);
    builder.DIBuilder().insertDeclare(
        value->Ptr(Where),
        d,
        builder.DIBuilder().createExpression(),
        llvm::DILocation::get(
            builder.IRContext(),
            Where.Row,
            Where.Col,
            &builder.DIScope()),
        builder.IRBuilder().GetInsertBlock()
    );

    return builder.Define(Where, Name) = value;
}
