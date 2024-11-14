#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::ReferenceType::GetName(const TypePtr& base_type)
{
    return base_type->Name + '&';
}

LX::ReferenceType::ReferenceType(TypePtr base_type)
    : Type(GetName(base_type), 64), BaseType(std::move(base_type))
{
}

bool LX::ReferenceType::IsReference() const
{
    return true;
}

LX::TypePtr& LX::ReferenceType::Base(const SourceLocation&)
{
    return BaseType;
}

llvm::Type* LX::ReferenceType::GenIR(const SourceLocation&, Builder& builder)
{
    if (m_IR) return m_IR;
    return m_IR = builder.IRBuilder().getPtrTy();
}

llvm::DIType* LX::ReferenceType::GenDI(Builder& builder)
{
    if (m_DI) return m_DI;
    return m_DI = builder.DIBuilder().createReferenceType(
        llvm::dwarf::DW_TAG_reference_type,
        BaseType->GenDI(builder));
}
