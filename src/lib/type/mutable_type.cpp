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

size_t LX::MutableType::IndexOf(const std::string& name) const
{
    return ElementType->IndexOf(name);
}

LX::TypePtr LX::MutableType::Element(const size_t i) const
{
    return ElementType->Element(i);
}

LX::TypePtr LX::MutableType::Element() const
{
    return ElementType;
}

llvm::Type* LX::MutableType::GenIR(Builder& builder) const
{
    return builder.IRBuilder().getPtrTy();
}
