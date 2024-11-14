#include <llvm/IR/DerivedTypes.h>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::ArrayType::GetName(const bool is_mutable, const TypePtr& base_type, const size_t size)
{
    std::string s;
    s += '[';
    if (is_mutable) s += "mut ";
    s += base_type->Name;
    s += ", ";
    s += std::to_string(size);
    s += ']';
    return s;
}

LX::ArrayType::ArrayType(const bool is_mutable, TypePtr base_type, const size_t size)
    : Type(GetName(is_mutable, base_type, size), base_type->Bits * size),
      Mutable(is_mutable),
      BaseType(std::move(base_type)),
      Size(size)
{
}

bool LX::ArrayType::IsArray() const
{
    return true;
}

bool LX::ArrayType::IsMutable() const
{
    return Mutable;
}

LX::TypePtr& LX::ArrayType::Base(const SourceLocation& where)
{
    return BaseType;
}

llvm::Type* LX::ArrayType::GenIR(const SourceLocation& where, Builder& builder)
{
    if (m_IR) return m_IR;
    return m_IR = llvm::ArrayType::get(BaseType->GenIR(where, builder), Size);
}

llvm::DIType* LX::ArrayType::GenDI(Builder& builder)
{
    if (m_DI) return m_DI;
    return m_DI = builder.DIBuilder().createArrayType(
        Size,
        0,
        BaseType->GenDI(builder),
        builder.DIBuilder().getOrCreateArray({
            builder.DIBuilder().getOrCreateSubrange(0, static_cast<int64_t>(Size))
        }));
}
