#include <llvm/IR/DerivedTypes.h>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::ArrayType::GetName(const TypePtr& element_type, const size_t size)
{
    return element_type->Name + '[' + std::to_string(size) + ']';
}

LX::ArrayType::ArrayType(TypePtr element_type, const size_t size)
    : Type(GetName(element_type, size), element_type->Bits * size), ElementType(std::move(element_type)), Size(size)
{
}

bool LX::ArrayType::IsArray() const
{
    return true;
}

LX::TypePtr LX::ArrayType::Element(const SourceLocation& where) const
{
    return ElementType;
}

LX::TypePtr LX::ArrayType::Element(const SourceLocation&, const size_t) const
{
    return ElementType;
}

llvm::Type* LX::ArrayType::GenIR(const SourceLocation& where, Builder& builder)
{
    if (m_IR) return m_IR;
    return m_IR = llvm::ArrayType::get(ElementType->GenIR(where, builder), Size);
}

llvm::DIType* LX::ArrayType::GenDI(Builder& builder)
{
    if (m_DI) return m_DI;
    return m_DI = builder.DIBuilder().createArrayType(Size, 0, ElementType->GenDI(builder), {});
}
