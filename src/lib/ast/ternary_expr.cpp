#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Value.hpp>

LX::TernaryExpr::TernaryExpr(SourceLocation where, TypePtr type, ExprPtr condition, ExprPtr then, ExprPtr else_)
    : Expr(std::move(where), std::move(type)),
      Condition(std::move(condition)),
      Then(std::move(then)),
      Else(std::move(else_))
{
}

std::ostream& LX::TernaryExpr::Print(std::ostream& os) const
{
    return Else->Print(Then->Print(Condition->Print(os) << " ? ") << " : ");
}

LX::ValuePtr LX::TernaryExpr::GenIR(Builder& builder) const
{
    const auto parent = builder.IRBuilder().GetInsertBlock()->getParent();
    auto then_bb = llvm::BasicBlock::Create(builder.IRContext(), "then", parent);
    auto else_bb = llvm::BasicBlock::Create(builder.IRContext(), "else", parent);
    const auto end_bb = llvm::BasicBlock::Create(builder.IRContext(), "end", parent);

    const auto condition = Condition->GenIR(builder);
    builder.IRBuilder().CreateCondBr(condition->Load(builder), then_bb, else_bb);

    builder.IRBuilder().SetInsertPoint(then_bb);
    auto then = Then->GenIR(builder);
    then_bb = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().CreateBr(end_bb);

    builder.IRBuilder().SetInsertPoint(else_bb);
    auto else_ = Else->GenIR(builder);
    else_bb = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().CreateBr(end_bb);

    builder.IRBuilder().SetInsertPoint(then_bb->getTerminator());
    then = builder.Cast(Where, then, Type);
    const auto then_value = then->Load(builder);
    builder.IRBuilder().SetInsertPoint(else_bb->getTerminator());
    else_ = builder.Cast(Where, else_, Type);
    const auto else_value = else_->Load(builder);

    builder.IRBuilder().SetInsertPoint(end_bb);
    const auto phi = builder.IRBuilder().CreatePHI(then_value->getType(), 2);
    phi->addIncoming(then_value, then_bb);
    phi->addIncoming(else_value, else_bb);

    return RValue::Create(Type, phi);
}
