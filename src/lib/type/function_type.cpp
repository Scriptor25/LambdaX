#include <llvm/IR/DerivedTypes.h>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::FunctionType::GetName(
    const TypePtr& result_type,
    const std::vector<Parameter>& params,
    const bool is_mutable,
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
    name += ')';
    if (!result_type->IsVoid())
    {
        name += " => ";
        if (is_mutable) name += "mut ";
        name += result_type->Name;
    }
    return name;
}

LX::FunctionType::FunctionType(
    TypePtr result_type,
    std::vector<Parameter> params,
    const bool is_mutable,
    const bool vararg)
    : Type(GetName(result_type, params, is_mutable, vararg), 0),
      ResultType(std::move(result_type)),
      Params(std::move(params)),
      Mutable(is_mutable),
      VarArg(vararg)
{
}

bool LX::FunctionType::IsFunction() const
{
    return true;
}

bool LX::FunctionType::IsMutable() const
{
    return Mutable;
}

LX::TypePtr& LX::FunctionType::Result(const SourceLocation&)
{
    return ResultType;
}

size_t LX::FunctionType::ParamCount(const SourceLocation&)
{
    return Params.size();
}

LX::Parameter& LX::FunctionType::Param(const SourceLocation&, const size_t index)
{
    return Params[index];
}

bool LX::FunctionType::HasVarArg(const SourceLocation&)
{
    return VarArg;
}

llvm::Type* LX::FunctionType::GenIR(const SourceLocation& where, Builder& builder)
{
    if (m_IR) return m_IR;

    const auto result_type = ResultType->GenIR(where, builder);
    std::vector<llvm::Type*> param_types;
    for (const auto& [mutable_, type_, name_] : Params)
        param_types.push_back(type_->GenIR(where, builder));
    return m_IR = llvm::FunctionType::get(result_type, param_types, VarArg);
}

llvm::DIType* LX::FunctionType::GenDI(Builder& builder)
{
    if (m_DI) return m_DI;

    std::vector<llvm::Metadata*> elements;
    elements.push_back(ResultType->GenDI(builder));
    for (const auto& [mutable_, type_, name_] : Params)
        elements.push_back(type_->GenDI(builder));
    return m_DI = builder.DIBuilder().createSubroutineType(builder.DIBuilder().getOrCreateTypeArray(elements));
}
