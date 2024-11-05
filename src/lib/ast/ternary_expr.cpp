#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Value.hpp>

#include "LX/Type.hpp"

LX::TernaryExpr::TernaryExpr(ExprPtr condition, ExprPtr then, ExprPtr else_)
    : Condition(std::move(condition)), Then(std::move(then)), Else(std::move(else_))
{
}

std::ostream& LX::TernaryExpr::Print(std::ostream& os) const
{
    return Else->Print(Then->Print(Condition->Print(os) << " ? ") << " : ");
}

void LX::TernaryExpr::GenIR(Builder& builder, Value& ref) const
{
    const auto parent = builder.IRBuilder().GetInsertBlock()->getParent();
    auto then_bb = llvm::BasicBlock::Create(builder.IRContext(), "then", parent);
    auto else_bb = llvm::BasicBlock::Create(builder.IRContext(), "else", parent);
    const auto end_bb = llvm::BasicBlock::Create(builder.IRContext(), "end", parent);

    Value condition;
    Condition->GenIR(builder, condition);
    if (!condition) Error("condition is null");
    builder.IRBuilder().CreateCondBr(condition.ValueIR, then_bb, else_bb);

    builder.IRBuilder().SetInsertPoint(then_bb);
    Value then;
    Then->GenIR(builder, then);
    if (!then) Error("then is null");
    then_bb = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().CreateBr(end_bb);

    builder.IRBuilder().SetInsertPoint(else_bb);
    Value else_;
    Else->GenIR(builder, else_);
    if (!else_) Error("else is null");
    else_bb = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().CreateBr(end_bb);

    const auto dst_ty = Type::Equalize(builder.Ctx(), then.Type, else_.Type);
    builder.IRBuilder().SetInsertPoint(then_bb->getTerminator());
    builder.Cast(then, dst_ty, then);
    builder.IRBuilder().SetInsertPoint(else_bb->getTerminator());
    builder.Cast(else_, dst_ty, else_);

    builder.IRBuilder().SetInsertPoint(end_bb);
    const auto phi = builder.IRBuilder().CreatePHI(then.ValueIR->getType(), 2);
    phi->addIncoming(then.ValueIR, then_bb);
    phi->addIncoming(else_.ValueIR, else_bb);

    ref.Type = then.Type;
    ref.ValueIR = phi;
}
