#include <LX/AST.hpp>

#include "LX/Builder.hpp"
#include "LX/Error.hpp"
#include "LX/Type.hpp"
#include "LX/Value.hpp"

LX::SwitchExpr::SwitchExpr(SourceLocation where, ExprPtr switch_, std::vector<Case> cases, ExprPtr default_)
    : Expr(std::move(where)), Switch(std::move(switch_)), Cases(std::move(cases)), Default(std::move(default_))
{
}

std::ostream& LX::SwitchExpr::Print(std::ostream& os) const
{
    Switch->Print(os << "switch (") << ") [ ";
    for (size_t i = 0; i < Cases.size(); ++i)
    {
        if (i > 0) os << ", ";
        const auto& [conditions_, then_] = Cases[i];
        for (size_t j = 0; j < conditions_.size(); ++j)
        {
            if (j > 0) os << ", ";
            os << conditions_[j];
        }
        then_->Print(os << ": ");
    }
    if (Default)
        Default->Print(os << (Cases.empty() ? "" : ", ") << "def: ");
    return os << " ]";
}

LX::ValuePtr LX::SwitchExpr::GenIR(Builder& builder) const
{
    const auto parent = builder.IRBuilder().GetInsertBlock()->getParent();

    auto switch_ = Switch->GenIR(builder);

    auto switch_type = switch_->Type();
    std::vector<llvm::BasicBlock*> blocks;
    std::vector<std::pair<ValuePtr, llvm::BasicBlock*>> cases;

    for (const auto& [conditions_, then_] : Cases)
    {
        const auto dest_block = llvm::BasicBlock::Create(builder.IRContext(), "case", parent);
        for (const auto& condition : conditions_)
        {
            auto cond = condition->GenIR(builder);
            switch_type = Type::Equalize(Where, builder.Ctx(), switch_type, cond->Type());
            cases.emplace_back(cond, dest_block);
        }
        blocks.push_back(dest_block);
    }

    auto default_block = llvm::BasicBlock::Create(builder.IRContext(), "default", parent);
    const auto end_block = llvm::BasicBlock::Create(builder.IRContext(), "end", parent);

    Where.EmitDI(builder);

    switch_ = builder.CreateCast(Where, switch_, switch_type);
    const auto ir_switch = builder.IRBuilder().CreateSwitch(
        switch_->Load(Where, builder),
        default_block,
        cases.size());

    for (const auto& [cond_, block_] : cases)
    {
        const auto cond = builder.CreateCast(Where, cond_, switch_type);
        const auto on_val = llvm::dyn_cast<llvm::ConstantInt>(cond->Load(Where, builder));
        if (!on_val)
            Error(Where, "case selector is not a constant integer expression");
        ir_switch->addCase(on_val, block_);
    }

    std::vector<ValuePtr> values;
    TypePtr result_type;

    for (size_t i = 0; i < Cases.size(); ++i)
    {
        builder.IRBuilder().SetInsertPoint(blocks[i]);
        const auto then = Cases[i].Then->GenIR(builder);
        result_type = !result_type
                          ? then->Type()
                          : Type::Equalize(Where, builder.Ctx(), result_type, then->Type());
        blocks[i] = builder.IRBuilder().GetInsertBlock();
        values.push_back(then);
    }

    if (Default)
    {
        builder.IRBuilder().SetInsertPoint(default_block);
        const auto default_ = Default->GenIR(builder);
        result_type = !result_type
                          ? default_->Type()
                          : Type::Equalize(Where, builder.Ctx(), result_type, default_->Type());
        default_block = builder.IRBuilder().GetInsertBlock();
        blocks.push_back(default_block);
        values.push_back(default_);
    }
    else
    {
        blocks.push_back(default_block);
        values.push_back({});
    }

    builder.IRBuilder().SetInsertPoint(end_block);
    const auto ir_type = result_type->GenIR(Where, builder);
    const auto ir_phi = builder.IRBuilder().CreatePHI(ir_type, Cases.size() + 1);

    for (size_t i = 0; i < blocks.size(); ++i)
    {
        builder.IRBuilder().SetInsertPoint(blocks[i]);
        llvm::Value* ir_value{};
        if (values[i])
        {
            values[i] = builder.CreateCast(Where, values[i], result_type);
            ir_value = values[i]->Load(Where, builder);
        }
        else ir_value = llvm::Constant::getNullValue(ir_type);
        builder.IRBuilder().CreateBr(end_block);

        ir_phi->addIncoming(ir_value, blocks[i]);
    }

    builder.IRBuilder().SetInsertPoint(end_block);
    return RValue::Create(result_type, ir_phi);
}
