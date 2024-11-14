#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::PointerType::GetName(const bool is_mutable, const TypePtr& base_type)
{
    std::string s;
    s += '[';
    if (is_mutable) s += "mut ";
    s += base_type->Name;
    s += ']';
    return s;
}

LX::PointerType::PointerType(const bool is_mutable, TypePtr base_type)
    : Type(GetName(is_mutable, base_type), 64), Mutable(is_mutable), BaseType(std::move(base_type))
{
}

bool LX::PointerType::IsPointer() const
{
    return true;
}

bool LX::PointerType::IsMutable() const
{
    return Mutable;
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
