#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::NewExpr::NewExpr(SourceLocation where, const bool is_mutable, TypePtr type, const size_t count)
    : Expr(std::move(where)), Mutable(is_mutable), Type(std::move(type)), Count(count)
{
}

std::ostream& LX::NewExpr::Print(std::ostream& os) const
{
    os << "new (";
    if (Mutable) os << "mut ";
    Type->Print(os);
    if (Count > 1) os << ", " << Count;
    return os << ')';
}

LX::ValuePtr LX::NewExpr::GenIR(Builder& builder) const
{
    Where.EmitDI(builder);

    const auto size = builder.IRBuilder().getInt64(Type->Bits * Count);
    const auto type = llvm::FunctionType::get(
        builder.IRBuilder().getPtrTy()
        , {builder.IRBuilder().getInt64Ty()},
        false);
    const auto malloc = builder.IRModule().getOrInsertFunction("malloc", type, {});
    const auto ptr = builder.IRBuilder().CreateCall(malloc, {size});
    return RValue::Create(builder.Ctx().GetPointerType(Mutable, Type), ptr);
}
