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

llvm::Type* LX::ArrayType::GenIR(Builder& builder) const
{
    return llvm::ArrayType::get(ElementType->GenIR(builder), Size);
}

llvm::DIType* LX::ArrayType::GenDI(Builder& builder) const
{
    return builder.DIBuilder().createArrayType(Size, 0, ElementType->GenDI(builder), {});
}
