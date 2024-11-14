#include <llvm/IR/DerivedTypes.h>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::ArrayType::GetName(const TypePtr& base_type, const size_t size)
{
    return '[' + base_type->Name + ", " + std::to_string(size) + ']';
}

LX::ArrayType::ArrayType(TypePtr base_type, const size_t size)
    : Type(GetName(base_type, size), base_type->Bits * size), BaseType(std::move(base_type)), Size(size)
{
}

bool LX::ArrayType::IsArray() const
{
    return true;
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
