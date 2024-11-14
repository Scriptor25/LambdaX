#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

LX::Context::Context()
{
    m_Types["u1"] = std::make_shared<IntType>(1, false);
    m_Types["u8"] = std::make_shared<IntType>(8, false);
    m_Types["u16"] = std::make_shared<IntType>(16, false);
    m_Types["u32"] = std::make_shared<IntType>(32, false);
    m_Types["u64"] = std::make_shared<IntType>(64, false);
    m_Types["i1"] = std::make_shared<IntType>(1, true);
    m_Types["i8"] = std::make_shared<IntType>(8, true);
    m_Types["i16"] = std::make_shared<IntType>(16, true);
    m_Types["i32"] = std::make_shared<IntType>(32, true);
    m_Types["i64"] = std::make_shared<IntType>(64, true);
    m_Types["f16"] = std::make_shared<FloatType>(16);
    m_Types["f32"] = std::make_shared<FloatType>(32);
    m_Types["f64"] = std::make_shared<FloatType>(64);
}

LX::TypePtr& LX::Context::GetType(const std::string& name)
{
    return m_Types[name];
}

LX::TypePtr& LX::Context::GetVoidType()
{
    auto& type = GetType("");
    if (type) return type;
    return type = std::make_shared<VoidType>();
}

LX::TypePtr& LX::Context::GetIntType(const unsigned bits, const bool sign)
{
    auto& type = GetType(IntType::GetName(bits, sign));
    if (type) return type;
    return type = std::make_shared<IntType>(bits, sign);
}

LX::TypePtr& LX::Context::GetFloatType(const unsigned bits)
{
    auto& type = GetType(FloatType::GetName(bits));
    if (type) return type;
    return type = std::make_shared<FloatType>(bits);
}

LX::TypePtr& LX::Context::GetStructType(
    const SourceLocation& where,
    const std::string& name,
    const std::vector<Parameter>& elements)
{
    auto& type = GetType(StructType::GetName(name, elements));
    if (type)
    {
        if (!elements.empty()) type->SetElements(where, elements);
        return type;
    }
    return type = std::make_shared<StructType>(name, elements);
}

LX::TypePtr& LX::Context::GetPointerType(const bool is_mutable, const TypePtr& element)
{
    auto& type = GetType(PointerType::GetName(is_mutable, element));
    if (type) return type;
    return type = std::make_shared<PointerType>(is_mutable, element);
}

LX::TypePtr& LX::Context::GetReferenceType(const TypePtr& element)
{
    auto& type = GetType(ReferenceType::GetName(element));
    if (type) return type;
    return type = std::make_shared<ReferenceType>(element);
}

LX::TypePtr& LX::Context::GetArrayType(const bool is_mutable, const TypePtr& element, const size_t size)
{
    auto& type = GetType(ArrayType::GetName(is_mutable, element, size));
    if (type) return type;
    return type = std::make_shared<ArrayType>(is_mutable, element, size);
}

LX::TypePtr& LX::Context::GetFunctionType(
    const TypePtr& result_type,
    const std::vector<Parameter>& params,
    const bool is_mutable,
    const bool vararg)
{
    auto& type = GetType(FunctionType::GetName(result_type, params, is_mutable, vararg));
    if (type) return type;
    return type = std::make_shared<FunctionType>(result_type, params, is_mutable, vararg);
}
