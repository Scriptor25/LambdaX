#include <ranges>
#include <llvm/IR/DerivedTypes.h>
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

    Push();
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

LX::TypePtr& LX::Context::GetStructType(const std::vector<Parameter>& elements)
{
    auto& type = GetType(StructType::GetName(elements));
    if (type) return type;
    return type = std::make_shared<StructType>(elements);
}

LX::TypePtr& LX::Context::GetPointerType(const TypePtr& element)
{
    auto& type = GetType(PointerType::GetName(element));
    if (type) return type;
    return type = std::make_shared<PointerType>(element);
}

LX::TypePtr& LX::Context::GetMutableType(const TypePtr& element)
{
    auto& type = GetType(MutableType::GetName(element));
    if (type) return type;
    return type = std::make_shared<MutableType>(element);
}

LX::TypePtr& LX::Context::GetArrayType(const TypePtr& element, const size_t size)
{
    auto& type = GetType(ArrayType::GetName(element, size));
    if (type) return type;
    return type = std::make_shared<ArrayType>(element, size);
}

LX::TypePtr& LX::Context::GetFunctionType(
    const TypePtr& result_type,
    const std::vector<Parameter>& params,
    const bool vararg)
{
    auto& type = GetType(FunctionType::GetName(result_type, params, vararg));
    if (type) return type;
    return type = std::make_shared<FunctionType>(result_type, params, vararg);
}

LX::TypePtr& LX::Context::GetFunPtrType(
    const TypePtr& result_type,
    const std::vector<Parameter>& params,
    const bool vararg)
{
    return GetPointerType(GetFunctionType(result_type, params, vararg));
}

void LX::Context::Push()
{
    m_Stack.emplace_back();
}

void LX::Context::Pop()
{
    m_Stack.pop_back();
}

LX::TypePtr& LX::Context::DefVar(const SourceLocation& where, const std::string& name)
{
    if (m_Stack.back().contains(name))
        Error(where, "cannot redefine symbol with name '{}'", name);
    return m_Stack.back()[name];
}

LX::TypePtr LX::Context::GetVar(const SourceLocation& where, const std::string& name)
{
    for (const auto& frame : std::ranges::views::reverse(m_Stack))
        if (frame.contains(name)) return frame.at(name);
    Error(where, "no symbol with name '{}'", name);
}

bool LX::Context::HasVar(const std::string& name)
{
    return std::ranges::any_of(
        std::ranges::views::reverse(m_Stack),
        [&](const auto& frame) { return frame.contains(name); });
}
