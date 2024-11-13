#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::DefineExpr::DefineExpr(SourceLocation where, const bool is_mutable, std::string name, TypePtr type, ExprPtr init)
    : Expr(std::move(where)), IsMutable(is_mutable), Name(std::move(name)), Type(std::move(type)), Init(std::move(init))
{
}

std::ostream& LX::DefineExpr::Print(std::ostream& os) const
{
    if (IsMutable) os << "mut ";
    os << Name;
    if (Type) Type->Print(os << " => ");
    if (Init) Init->Print(os << " := ");
    return os;
}

LX::ValuePtr LX::DefineExpr::GenIR(Builder& builder) const
{
    auto init = Init ? Init->GenIR(builder) : nullptr;
    auto type = Type ? Type : init->Type();

    Where.EmitDI(builder);

    ValuePtr value;
    if (type->IsReference())
    {
        if (!init) Error(Where, "reference definition requires initializer");
        type = type->Element(Where);
        if (type != init->Type())
            Error(
                Where,
                "cannot assign value of type {} to reference value of type {}; casting would create a copy",
                init->Type(),
                type);
        if (IsMutable && !init->IsMutable())
            Error(
                Where,
                "cannot assign immutable reference to mutable reference; discarding mutability");
        value = LValue::Create(type, init->Ptr(Where), IsMutable);
    }
    else
    {
        value = builder.CreateAlloca(Where, type, IsMutable, Name);
        if (init)
        {
            init = builder.CreateCast(Where, init, type);
            value->StoreForce(Where, builder, init->Load(Where, builder));
        }
    }

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
