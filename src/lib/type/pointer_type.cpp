#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::PointerType::GetName(const TypePtr& base_type)
{
    return '[' + base_type->Name + ']';
}

LX::PointerType::PointerType(TypePtr base_type)
    : Type(GetName(base_type), 64), BaseType(std::move(base_type))
{
}

bool LX::PointerType::IsPointer() const
{
    return true;
}

LX::TypePtr& LX::PointerType::Base(const SourceLocation&)
{
    return BaseType;
}

llvm::Type* LX::PointerType::GenIR(const SourceLocation&, Builder& builder)
{
    if (m_IR) return m_IR;
    return m_IR = builder.IRBuilder().getPtrTy();
}

llvm::DIType* LX::PointerType::GenDI(Builder& builder)
{
    if (m_DI) return m_DI;
    return m_DI = builder.DIBuilder().createPointerType(BaseType->GenDI(builder), Bits);
}
