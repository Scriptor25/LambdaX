#include <llvm/IR/DerivedTypes.h>
#include <LX/Type.hpp>

std::string LX::FunctionType::GetName(
    const TypePtr& result_type,
    const std::vector<TypePtr>& param_types,
    const bool vararg)
{
    std::string name = "(";
    for (size_t i = 0; i < param_types.size(); ++i)
    {
        if (i > 0) name += ", ";
        name += param_types[i]->Name;
    }
    if (vararg)
    {
        if (!param_types.empty()) name += ", ";
        name += "...";
    }
    return name + ") => " + result_type->Name;
}

LX::FunctionType::FunctionType(TypePtr result_type, std::vector<TypePtr> param_types, const bool vararg)
    : Type(GetName(result_type, param_types, vararg), 0),
      ResultType(std::move(result_type)),
      ParamTypes(std::move(param_types)),
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
    return ParamTypes.size();
}

LX::TypePtr LX::FunctionType::Param(const size_t index) const
{
    return ParamTypes[index];
}

bool LX::FunctionType::HasVarArg() const
{
    return VarArg;
}

llvm::Type* LX::FunctionType::GenIR(Builder& builder) const
{
    const auto result_type = ResultType->GetIR(builder);
    std::vector<llvm::Type*> param_types(ParamTypes.size());
    for (size_t i = 0; i < ParamTypes.size(); ++i)
        param_types[i] = ParamTypes[i]->GetIR(builder);
    return llvm::FunctionType::get(result_type, param_types, VarArg);
}
