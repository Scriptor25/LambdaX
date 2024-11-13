#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ImportStmt::ImportStmt(
    SourceLocation where,
    std::vector<FunctionImport> imports,
    std::string module_id,
    std::string name)
    : Stmt(std::move(where)),
      Imports(std::move(imports)),
      ModuleId(std::move(module_id)),
      Name(std::move(name))
{
}

std::ostream& LX::ImportStmt::Print(std::ostream& os) const
{
    return os << "import " << ModuleId << " as " << Name;
}

LX::ValuePtr LX::ImportStmt::GenIR(Builder& builder) const
{
    Where.EmitDI(builder);

    std::vector<llvm::Constant*> elements;
    for (const auto& [type_, name_, extern_] : Imports)
    {
        const auto function_type = llvm::dyn_cast<llvm::FunctionType>(type_->Element(Where)->GenIR(Where, builder));
        const auto name = !extern_ ? ModuleId + '.' + name_ : name_;

        const auto function = llvm::Function::Create(
            function_type,
            llvm::GlobalValue::ExternalLinkage,
            name,
            builder.IRModule());
        elements.push_back(function);
    }

    std::vector<Parameter> params(Imports.size());
    for (size_t i = 0; i < Imports.size(); ++i)
        params[i] = {Imports[i].Type, Imports[i].Name};

    const auto type = builder.Ctx().GetStructType(Where, "module." + Name, params);
    const auto struct_type = llvm::dyn_cast<llvm::StructType>(type->GenIR(Where, builder));
    const auto value = llvm::ConstantStruct::get(struct_type, elements);
    return builder.Define(Where, Name) = RValue::Create(type, value);
}
