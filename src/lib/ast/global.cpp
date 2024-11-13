#include <LX/AST.hpp>
#include <LX/Type.hpp>

#include "LX/Builder.hpp"
#include "LX/Value.hpp"

std::ostream& LX::GlobalSymbol::Print(std::ostream& os) const
{
    if (Export) os << "export ";
    if (Extern) os << "extern ";
    if (IsMutable) os << "mut ";
    os << Name << ' ';
    if (Type) Type->Print(os << "=> ");
    if (Init) Init->Print(os << "= ");
    return os;
}

LX::ValuePtr LX::GlobalSymbol::GenIR(const SourceLocation& where, Builder& builder) const
{
    auto init = Init ? Init->GenIR(builder) : nullptr;
    const auto type = Type ? Type : init->Type();
    const auto type_ir = type->GenIR(where, builder);

    if (init && Type)
        init = builder.CreateCast(where, init, type);

    const auto linkage = Export || Extern
                             ? llvm::GlobalValue::ExternalLinkage
                             : llvm::GlobalValue::InternalLinkage;
    const auto name = Export && !Extern
                          ? builder.ModuleId() + '.' + Name
                          : Name;
    const auto external_init = !Init && !IsMutable;

    const auto init_ir = init ? init->Load(where, builder) : nullptr;
    auto const_init_ir = init ? llvm::dyn_cast<llvm::Constant>(init_ir) : nullptr;
    const bool is_const = const_init_ir;
    if (!is_const) const_init_ir = llvm::Constant::getNullValue(type_ir);

    const auto ptr = new llvm::GlobalVariable(
        builder.IRModule(),
        type_ir,
        is_const,
        linkage,
        const_init_ir,
        name,
        nullptr,
        llvm::GlobalValue::NotThreadLocal,
        std::nullopt,
        external_init);

    const auto value = LValue::Create(type, ptr, IsMutable);
    if (!is_const && init_ir)
        value->StoreForce(where, builder, init_ir);

    return builder.Define(where, Name) = value;
}
