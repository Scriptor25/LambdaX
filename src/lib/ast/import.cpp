#include <llvm/IR/DerivedTypes.h>
#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

llvm::Constant* LX::FunctionImport::Resolve(
    const SourceLocation& where,
    Builder& builder,
    const std::string& module_id) const
{
    const auto type = Type->Element(where);
    const auto type_ir = llvm::dyn_cast<llvm::FunctionType>(type->GenIR(where, builder));
    const auto name = !Extern ? module_id + '.' + Name : Name;

    return llvm::Function::Create(
        type_ir,
        llvm::GlobalValue::ExternalLinkage,
        name,
        builder.IRModule());
}

llvm::Constant* LX::GlobalImport::Resolve(
    const SourceLocation& where,
    Builder& builder,
    const std::string& module_id) const
{
    const auto type_ir = Type->Element(where)->GenIR(where, builder);
    const auto name = !Extern ? module_id + '.' + Name : Name;

    return new llvm::GlobalVariable(
        builder.IRModule(),
        type_ir,
        !IsMutable,
        llvm::GlobalValue::ExternalLinkage,
        nullptr,
        name,
        nullptr,
        llvm::GlobalValue::NotThreadLocal,
        std::nullopt,
        true);
}
