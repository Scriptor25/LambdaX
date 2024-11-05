#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

LX::ConstFunctionExpr::ConstFunctionExpr(TypePtr type, std::vector<Parameter> params, ExprPtr body)
    : Type(std::move(type)), Params(std::move(params)), Body(std::move(body))
{
}

std::ostream& LX::ConstFunctionExpr::Print(std::ostream& os) const
{
    os << '$';
    if (!Params.empty())
    {
        os << '(';
        for (size_t i = 0; i < Params.size(); ++i)
        {
            if (i > 0) os << ", ";
            Params[i].Print(os);
        }
        os << ')';
    }
    return Body->Print(Type->Result()->Print(os << " => ") << " = ");
}

void LX::ConstFunctionExpr::GenIR(Builder& builder, Value& ref) const
{
    const auto type = llvm::dyn_cast<llvm::FunctionType>(Type->GenIR(builder));
    const auto function = llvm::Function::Create(type, llvm::GlobalValue::ExternalLinkage, "", builder.IRModule());

    ref.Type = builder.Ctx().GetPointerType(Type);
    ref.ValueIR = function;

    builder.Push();
    const auto bkp = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().SetInsertPoint(llvm::BasicBlock::Create(builder.IRContext(), "entry", function));
    for (size_t i = 0; i < Params.size(); ++i)
    {
        const auto& [param_type_, param_name_] = Params[i];

        const auto arg = function->getArg(i);
        arg->setName(param_name_);

        auto& [type_, value_ir_] = builder.DefVar(param_name_);
        type_ = param_type_;
        value_ir_ = arg;
    }

    Value value;
    Body->GenIR(builder, value);
    if (!value) Error("return value is null");
    builder.Cast(value, Type->Result(), value);
    builder.IRBuilder().CreateRet(value.ValueIR);
    builder.IRBuilder().SetInsertPoint(bkp);
    builder.Pop();

    if (verifyFunction(*function, &llvm::errs()))
        Error("failed to verify function lambda");
}
