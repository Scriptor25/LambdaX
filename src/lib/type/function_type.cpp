#include <llvm/IR/DerivedTypes.h>
#include <LX/Type.hpp>

std::string LX::FunctionType::GetName(
    const TypePtr& result_type,
    const std::vector<Parameter>& params,
    const bool vararg)
{
    std::string name = "(";
    for (size_t i = 0; i < params.size(); ++i)
    {
        if (i > 0) name += ", ";
        name += params[i].Type->Name;
    }
    if (vararg)
    {
        if (!params.empty()) name += ", ";
        name += "...";
    }
    return name + ") => " + result_type->Name;
}

LX::FunctionType::FunctionType(TypePtr result_type, std::vector<Parameter> params, const bool vararg)
    : Type(GetName(result_type, params, vararg), 0),
      ResultType(std::move(result_type)),
      Params(std::move(params)),
      VarArg(vararg)
{
}

bool LX::FunctionType::IsFunction() const
{
    return true;
}

LX::TypePtr LX::FunctionType::Result() const
{
    return ResultType;
}

size_t LX::FunctionType::ParamCount() const
{
    return Params.size();
}

LX::TypePtr LX::FunctionType::Param(const size_t index) const
{
    return Params[index].Type;
}

bool LX::FunctionType::HasVarArg() const
{
    return VarArg;
}

llvm::Type* LX::FunctionType::GenIR(Builder& builder) const
{
    const auto result_type = ResultType->GetIR(builder);
    std::vector<llvm::Type*> param_types(Params.size());
    for (size_t i = 0; i < Params.size(); ++i)
        param_types[i] = Params[i].Type->GetIR(builder);
    return llvm::FunctionType::get(result_type, param_types, VarArg);
}
