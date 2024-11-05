#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::PointerType::GetName(const TypePtr& element_type)
{
    if (!element_type) return "[]";
    return '[' + element_type->Name + ']';
}

LX::PointerType::PointerType(TypePtr element_type)
    : Type(GetName(element_type), 8), ElementType(std::move(element_type))
{
}

bool LX::PointerType::IsPointer() const
{
    return true;
}

LX::TypePtr LX::PointerType::Element() const
{
    return ElementType;
}

llvm::Type* LX::PointerType::GenIR(Builder& builder) const
{
    return builder.IRBuilder().getPtrTy();
}
