#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Type.hpp>

LX::ImportStmt::ImportStmt(std::vector<Parameter> imports, std::string name)
    : Imports(std::move(imports)), Name(std::move(name))
{
}

std::ostream& LX::ImportStmt::Print(std::ostream& os) const
{
    return os << "import " << Name;
}

LX::ValuePtr LX::ImportStmt::GenIR(Builder& builder) const
{
    std::vector<llvm::Constant*> elements;
    for (const auto& [type_, name_] : Imports)
    {
        const auto function_type = llvm::dyn_cast<llvm::FunctionType>(type_->Element()->GetIR(builder));
        const auto function = llvm::Function::Create(
            function_type,
            llvm::GlobalValue::ExternalLinkage,
            name_,
            builder.IRModule());
        elements.push_back(function);
    }

    const auto type = builder.Ctx().GetStructType(Imports);
    const auto struct_type = llvm::dyn_cast<llvm::StructType>(type->GetIR(builder));
    const auto value = llvm::ConstantStruct::get(struct_type, elements);
    return builder.DefVar(Name) = RValue::Create(type, value);
}
