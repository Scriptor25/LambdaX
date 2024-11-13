#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::ReferenceType::GetName(const TypePtr& element_type)
{
    return element_type->Name + '&';
}

LX::ReferenceType::ReferenceType(TypePtr element_type)
    : Type(GetName(element_type), 64), ElementType(std::move(element_type))
{
}

bool LX::ReferenceType::IsReference() const
{
    return true;
}

LX::TypePtr LX::ReferenceType::Element(const SourceLocation&) const
{
    return ElementType;
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
        ElementType->GenDI(builder));
}
