#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ImportStmt::ImportStmt(
    SourceLocation where,
    std::vector<ImportPtr> imports,
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
    std::vector<llvm::Constant*> elements;
    for (const auto& i : Imports)
        elements.push_back(i->Resolve(Where, builder, ModuleId));

    std::vector<Parameter> params(Imports.size());
    for (size_t i = 0; i < Imports.size(); ++i)
        params[i] = {Imports[i]->IsMutable, Imports[i]->Type, Imports[i]->Name};

    const auto type = builder.Ctx().GetStructType(Where, "module." + ModuleId, params);
    const auto struct_type = llvm::dyn_cast<llvm::StructType>(type->GenIR(Where, builder));
    const auto value = llvm::ConstantStruct::get(struct_type, elements);
    return builder.Define(Where, Name) = RValue::Create(type, value);
}
