#include <llvm/IR/Verifier.h>
#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::FunctionStmt::FunctionStmt(TypePtr type, std::string name, std::vector<Parameter> params, ExprPtr body)
    : Type(std::move(type)), Name(std::move(name)), Params(std::move(params)), Body(std::move(body))
{
}

std::ostream& LX::FunctionStmt::Print(std::ostream& os) const
{
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

void LX::FunctionStmt::GenIR(Builder& builder, Value& ref) const
{
    auto function = builder.IRModule().getFunction(Name);
    if (!function)
    {
        const auto type = llvm::dyn_cast<llvm::FunctionType>(Type->GenIR(builder));
        function = llvm::Function::Create(type, llvm::GlobalValue::ExternalLinkage, Name, builder.IRModule());
    }

    {
        auto& [type, value_ir] = builder.DefVar(Name);
        type = ref.Type = builder.Ctx().GetPointerType(Type);
        value_ir = ref.ValueIR = function;
    }

    if (!Body) return;
    if (!function->empty())
        Error("cannot redefine function '{}'", Name);

    builder.Push();
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
    builder.IRBuilder().ClearInsertionPoint();
    builder.Pop();

    if (verifyFunction(*function, &llvm::errs()))
        Error("failed to verify function '{}'", Name);
}
