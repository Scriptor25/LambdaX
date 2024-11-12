#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Value.hpp>

#include "LX/Type.hpp"

LX::TernaryExpr::TernaryExpr(SourceLocation where, ExprPtr condition, ExprPtr then, ExprPtr else_)
    : Expr(std::move(where)),
      Condition(std::move(condition)),
      Then(std::move(then)),
      Else(std::move(else_))
{
}

std::ostream& LX::TernaryExpr::Print(std::ostream& os) const
{
    if (!Else) return Then->Print(Condition->Print(os) << " ?? ");
    return Else->Print(Then->Print(Condition->Print(os) << " ? ") << " : ");
}

LX::ValuePtr LX::TernaryExpr::GenIR(Builder& builder) const
{
    const auto parent = builder.IRBuilder().GetInsertBlock()->getParent();
    auto then_bb = llvm::BasicBlock::Create(builder.IRContext(), "then", parent);
    auto else_bb = llvm::BasicBlock::Create(builder.IRContext(), "else", parent);
    const auto end_bb = llvm::BasicBlock::Create(builder.IRContext(), "end", parent);

    const auto condition = Condition->GenIR(builder);
    const auto cond = builder.IRBuilder().CreateIsNotNull(condition->Load(builder));
    builder.IRBuilder().CreateCondBr(cond, then_bb, else_bb);

    Where.EmitDI(builder);

    builder.IRBuilder().SetInsertPoint(then_bb);
    auto then = Then->GenIR(builder);
    then_bb = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().CreateBr(end_bb);

    builder.IRBuilder().SetInsertPoint(else_bb);
    auto else_ = Else ? Else->GenIR(builder) : nullptr;
    else_bb = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().CreateBr(end_bb);

    const auto type = else_
                          ? Type::Equalize(Where, builder.Ctx(), then->Type(), else_->Type())
                          : then->Type();

    builder.IRBuilder().SetInsertPoint(then_bb->getTerminator());
    then = builder.CreateCast(Where, then, type);
    const auto then_value = then->Load(builder);
    if (Else)
    {
        builder.IRBuilder().SetInsertPoint(else_bb->getTerminator());
        else_ = builder.CreateCast(Where, else_, type);
    }
    const auto else_value = Else
                                ? else_->Load(builder)
                                : llvm::Constant::getNullValue(then_value->getType());

    builder.IRBuilder().SetInsertPoint(end_bb);
    const auto phi = builder.IRBuilder().CreatePHI(then_value->getType(), 2);
    phi->addIncoming(then_value, then_bb);
    phi->addIncoming(else_value, else_bb);

    return RValue::Create(type, phi);
}
