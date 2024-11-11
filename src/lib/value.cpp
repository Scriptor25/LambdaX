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

llvm::Value* LX::RValue::Load(Builder&) const
{
    return m_Value;
}

void LX::RValue::Store(Builder&, llvm::Value*) const
{
    Error("cannot assign to rvalue");
}

llvm::Value* LX::RValue::Ptr() const
{
    Error("cannot get pointer to rvalue");
}

bool LX::RValue::IsMutable() const
{
    return false;
}

LX::RValue::RValue(TypePtr type, llvm::Value* value)
    : Value(std::move(type)), m_Value(value)
{
}

LX::ValuePtr LX::LValue::Create(const TypePtr& type, llvm::Value* ptr)
{
    return std::shared_ptr<LValue>(new LValue(type, ptr));
}

llvm::Value* LX::LValue::Load(Builder& builder) const
{
    return builder.IRBuilder().CreateLoad(m_Type->GenIR(builder), m_Ptr);
}

void LX::LValue::Store(Builder& builder, llvm::Value* value) const
{
    builder.IRBuilder().CreateStore(value, m_Ptr);
}

llvm::Value* LX::LValue::Ptr() const
{
    return m_Ptr;
}

bool LX::LValue::IsMutable() const
{
    return true;
}

LX::LValue::LValue(TypePtr type, llvm::Value* ptr)
    : Value(std::move(type)), m_Ptr(ptr)
{
}
