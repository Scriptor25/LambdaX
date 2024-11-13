#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::MutableType::GetName(const TypePtr& element_type)
{
    return "mut " + element_type->Name;
}

LX::MutableType::MutableType(TypePtr element_type)
    : Type(GetName(element_type), 64), ElementType(std::move(element_type))
{
}

bool LX::MutableType::IsMutable() const
{
    return true;
}

size_t LX::MutableType::IndexOf(const SourceLocation& where, const std::string& name) const
{
    return ElementType->IndexOf(where, name);
}

LX::TypePtr LX::MutableType::Element(const SourceLocation& where, const size_t i) const
{
    return ElementType->Element(where, i);
}

LX::TypePtr LX::MutableType::Element(const SourceLocation&) const
{
    return ElementType;
}

llvm::Type* LX::MutableType::GenIR(const SourceLocation&, Builder& builder)
{
    if (m_IR) return m_IR;
    return m_IR = builder.IRBuilder().getPtrTy();
}

llvm::DIType* LX::MutableType::GenDI(Builder& builder)
{
    if (m_DI) return m_DI;
    return m_DI = builder.DIBuilder().createReferenceType(
        llvm::dwarf::DW_TAG_reference_type,
        ElementType->GenDI(builder));
}
