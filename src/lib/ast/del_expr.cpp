#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Value.hpp>

LX::DelExpr::DelExpr(SourceLocation where, ExprPtr ptr)
    : Expr(std::move(where)), Ptr(std::move(ptr))
{
}

std::ostream& LX::DelExpr::Print(std::ostream& os) const
{
    return Ptr->Print(os << "del ");
}

LX::ValuePtr LX::DelExpr::GenIR(Builder& builder) const
{
    Where.EmitDI(builder);

    const auto ptr = Ptr->GenIR(builder)->Load(Where, builder);
    const auto type = llvm::FunctionType::get(
        builder.IRBuilder().getVoidTy()
        , {builder.IRBuilder().getPtrTy()},
        false);
    const auto free = builder.IRModule().getOrInsertFunction("free", type, {});
    const auto result = builder.IRBuilder().CreateCall(free, {ptr});
    return RValue::Create(builder.Ctx().GetVoidType(), result);
}
