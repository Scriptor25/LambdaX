#include <ranges>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

LX::TypePtr& LX::Context::StackFrame::operator[](const std::string& name)
{
    return Variables[name];
}

const LX::TypePtr& LX::Context::StackFrame::operator[](const std::string& name) const
{
    return Variables.at(name);
}

bool LX::Context::StackFrame::contains(const std::string& name) const
{
    return Variables.contains(name);
}

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

LX::TypePtr LX::Context::GetIntType(const unsigned bits, const bool sign)
{
    auto& type = GetType(IntType::GetName(bits, sign));
    if (type) return type;
    return type = std::make_shared<IntType>(bits, sign);
}

LX::TypePtr LX::Context::GetPointerType(const TypePtr& base)
{
    auto& type = GetType(PointerType::GetName(base));
    if (type) return type;
    return type = std::make_shared<PointerType>(base);
}

LX::TypePtr LX::Context::GetFunctionType(const TypePtr& result_type, const std::vector<TypePtr>& param_types)
{
    auto& type = GetType(FunctionType::GetName(result_type, param_types));
    if (type) return type;
    return type = std::make_shared<FunctionType>(result_type, param_types);
}

LX::TypePtr LX::Context::GetFunPtrType(const TypePtr& result_type, const std::vector<TypePtr>& param_types)
{
    return GetPointerType(GetFunctionType(result_type, param_types));
}

void LX::Context::Push()
{
    m_Stack.emplace_back();
}

void LX::Context::Pop()
{
    m_Stack.pop_back();
}

LX::TypePtr& LX::Context::DefVar(const std::string& name)
{
    if (m_Stack.back().contains(name))
        Error("cannot redefine variable '{}'", name);
    return m_Stack.back()[name];
}

const LX::TypePtr& LX::Context::GetVar(const std::string& name)
{
    for (const auto& it : std::ranges::views::reverse(m_Stack))
        if (it.contains(name)) return it[name];
    Error("undefined variable '{}'", name);
}
