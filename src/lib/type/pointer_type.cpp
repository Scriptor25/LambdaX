#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::PointerType::GetName(const TypePtr& element_type)
{
    return '[' + element_type->Name + ']';
}

LX::PointerType::PointerType(TypePtr element_type)
    : Type(GetName(element_type), 64), ElementType(std::move(element_type))
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

llvm::Type* LX::PointerType::GenIR(Builder& builder)
{
    if (m_IR) return m_IR;
    return m_IR = builder.IRBuilder().getPtrTy();
}

llvm::DIType* LX::PointerType::GenDI(Builder& builder)
{
    if (m_DI) return m_DI;
    return m_DI = builder.DIBuilder().createPointerType(ElementType->GenDI(builder), Bits);
}
