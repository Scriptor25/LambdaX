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
    const auto value = builder.CreateAlloca(type, true, Name);

    if (Init)
    {
        if (Type) init = builder.CreateCast(Where, init, Type);
        value->Store(builder, init->Load(builder));
    }

    const auto d = builder.DIBuilder().createAutoVariable(
        &builder.DIScope(),
        Name,
        builder.DIScope().getFile(),
        Where.Row,
        type->GenDI(builder),
        true);
    builder.DIBuilder().insertDeclare(
        value->Ptr(),
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
