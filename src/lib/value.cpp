#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::TypePtr LX::Value::Type() const
{
    return m_Type;
}

LX::Value::Value(TypePtr type)
    : m_Type(std::move(type))
{
}

LX::ValuePtr LX::RValue::Create(const TypePtr& type, llvm::Value* value)
{
    return std::shared_ptr<RValue>(new RValue(type, value));
}

llvm::Value* LX::RValue::Load(const SourceLocation&, Builder&) const
{
    return m_Value;
}

void LX::RValue::Store(const SourceLocation& where, Builder&, llvm::Value*) const
{
    Error(where, "cannot assign to rvalue");
}

void LX::RValue::StoreForce(const SourceLocation& where, Builder&, llvm::Value*) const
{
    Error(where, "cannot assign to rvalue");
}

llvm::Value* LX::RValue::Ptr(const SourceLocation& where) const
{
    Error(where, "cannot get pointer to rvalue");
}

bool LX::RValue::IsMutable() const
{
    return false;
}

bool LX::RValue::IsReference() const
{
    return false;
}

LX::RValue::RValue(TypePtr type, llvm::Value* value)
    : Value(std::move(type)), m_Value(value)
{
}

LX::ValuePtr LX::LValue::Create(const TypePtr& type, llvm::Value* ptr, const bool is_mutable)
{
    return std::shared_ptr<LValue>(new LValue(type, ptr, is_mutable));
}

llvm::Value* LX::LValue::Load(const SourceLocation& where, Builder& builder) const
{
    return builder.IRBuilder().CreateLoad(m_Type->GenIR(where, builder), m_Ptr);
}

void LX::LValue::Store(const SourceLocation& where, Builder& builder, llvm::Value* value) const
{
    if (!m_IsMutable)
        Error(where, "lvalue is not marked as mutable");
    builder.IRBuilder().CreateStore(value, m_Ptr);
}

void LX::LValue::StoreForce(const SourceLocation&, Builder& builder, llvm::Value* value) const
{
    builder.IRBuilder().CreateStore(value, m_Ptr);
}

llvm::Value* LX::LValue::Ptr(const SourceLocation&) const
{
    return m_Ptr;
}

bool LX::LValue::IsMutable() const
{
    return m_IsMutable;
}

bool LX::LValue::IsReference() const
{
    return true;
}

LX::LValue::LValue(TypePtr type, llvm::Value* ptr, const bool is_mutable)
    : Value(std::move(type)), m_Ptr(ptr), m_IsMutable(is_mutable)
{
}
