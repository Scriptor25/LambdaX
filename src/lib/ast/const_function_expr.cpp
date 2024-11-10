#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

LX::ConstFunctionExpr::ConstFunctionExpr(
    SourceLocation where,
    TypePtr type,
    std::vector<Parameter> params,
    ExprPtr body)
    : Expr(std::move(where), std::move(type)), Params(std::move(params)), Body(std::move(body))
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
    return Body->Print(Type->Element()->Result()->Print(os << " => ") << " = ");
}

LX::ValuePtr LX::ConstFunctionExpr::GenIR(Builder& builder) const
{
    const auto type = llvm::dyn_cast<llvm::FunctionType>(Type->Element()->GetIR(builder));
    const auto function = llvm::Function::Create(
        type,
        llvm::GlobalValue::InternalLinkage,
        "lambda",
        builder.IRModule());

    builder.Push();
    const auto bkp = builder.IRBuilder().GetInsertBlock();
    builder.IRBuilder().SetInsertPoint(llvm::BasicBlock::Create(builder.IRContext(), "entry", function));
    for (size_t i = 0; i < Params.size(); ++i)
    {
        const auto& [param_type_, param_name_] = Params[i];

        const auto arg = function->getArg(i);
        arg->setName(param_name_);

        builder.DefVar(Where, param_name_) =
            param_type_->IsMutable()
                ? LValue::Create(param_type_->Element(), arg)
                : RValue::Create(param_type_, arg);
    }

    auto value = Body->GenIR(builder);
    value = builder.Cast(Where, value, Type->Element()->Result());
    builder.IRBuilder().CreateRet(
        Type->Element()->Result()->IsMutable()
            ? value->Ptr()
            : value->Load(builder));
    builder.IRBuilder().SetInsertPoint(bkp);
    builder.Pop();

    if (verifyFunction(*function, &llvm::errs()))
        Error(Where, "failed to verify lambda function");

    return RValue::Create(Type, function);
}
