#include <llvm/IR/Verifier.h>
#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::FunctionStmt::FunctionStmt(
    const bool export_,
    TypePtr type,
    std::string name,
    std::vector<Parameter> params,
    ExprPtr body)
    : Export(export_), Type(std::move(type)), Name(std::move(name)), Params(std::move(params)), Body(std::move(body))
{
}

std::ostream& LX::FunctionStmt::Print(std::ostream& os) const
{
    if (Export) os << "export ";
    os << Name << '(';
    for (size_t i = 0; i < Params.size(); ++i)
    {
        if (i > 0) os << ", ";
        Params[i].Print(os);
    }
    Type->Result()->Print(os << ") => ");
    if (Body) Body->Print(os << " = ");
    return os;
}

LX::ValuePtr LX::FunctionStmt::GenIR(Builder& builder) const
{
    llvm::Function* function;
    ValuePtr result;

    if (!builder.HasVar(Name))
    {
        const auto type = llvm::dyn_cast<llvm::FunctionType>(Type->GetIR(builder));
        function = llvm::Function::Create(
            type,
            Export
                ? llvm::GlobalValue::ExternalLinkage
                : llvm::GlobalValue::InternalLinkage,
            Name,
            builder.IRModule());
        result = RValue::Create(builder.Ctx().GetPointerType(Type), function);
        builder.DefVar(Name) = result;
    }
    else
    {
        const auto var = builder.GetVar(Name);
        const auto val = var->Load(builder);
        function = llvm::dyn_cast<llvm::Function>(val);
        result = var;
    }

    if (!Body) return result;
    if (!function->empty())
        Error("cannot redefine function '{}'", Name);

    builder.Push();
    builder.IRBuilder().SetInsertPoint(llvm::BasicBlock::Create(builder.IRContext(), "entry", function));
    for (size_t i = 0; i < Params.size(); ++i)
    {
        const auto& [param_type_, param_name_] = Params[i];

        const auto arg = function->getArg(i);
        arg->setName(param_name_);

        builder.DefVar(param_name_) =
            param_type_->IsMutable()
                ? LValue::Create(param_type_->Element(), arg)
                : RValue::Create(param_type_, arg);
    }

    auto value = Body->GenIR(builder);
    value = builder.Cast(value, Type->Result());
    builder.IRBuilder().CreateRet(
        Type->Result()->IsMutable()
            ? value->Ptr()
            : value->Load(builder));
    builder.IRBuilder().ClearInsertionPoint();
    builder.Pop();

    if (verifyFunction(*function, &llvm::errs()))
        Error("failed to verify function '{}'", Name);

    return result;
}
