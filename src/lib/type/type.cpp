#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

LX::TypePtr LX::Type::Equalize(Context& ctx, const TypePtr& a, const TypePtr& b)
{
    if (a == b)
        return a;

    if (a->IsInt() && b->IsInt())
    {
        const auto bits = std::max(a->Bits, b->Bits);
        const auto sign = a->IsSigned() || b->IsSigned();
        return ctx.GetIntType(bits, sign);
    }

    if (a->IsFloat() || b->IsFloat())
    {
        const auto bits = std::max(a->Bits, b->Bits);
        return ctx.GetFloatType(bits);
    }

    return {};
}

LX::Type::Type(std::string name, const unsigned bits)
    : Name(std::move(name)), Bits(bits)
{
}

std::ostream& LX::Type::Print(std::ostream& os) const
{
    return os << Name;
}

bool LX::Type::IsInt() const
{
    return false;
}

bool LX::Type::IsSigned() const
{
    return false;
}

bool LX::Type::IsFloat() const
{
    return false;
}

bool LX::Type::IsPointer() const
{
    return false;
}

bool LX::Type::IsFunction() const
{
    return false;
}

bool LX::Type::IsMutable() const
{
    return false;
}

LX::TypePtr LX::Type::Result() const
{
    Error("type '{}' does not override Result", Name);
}

size_t LX::Type::ParamCount() const
{
    Error("type '{}' does not override ParamCount", Name);
}

LX::TypePtr LX::Type::Param(size_t) const
{
    Error("type '{}' does not override Param", Name);
}

bool LX::Type::HasVarArg() const
{
    Error("type '{}' does not override HasVarArg", Name);
}

llvm::Type* LX::Type::GetIR(Builder& builder)
{
    if (IR) return IR;
    return IR = GenIR(builder);
}

LX::TypePtr LX::Type::Element() const
{
    Error("type '{}' does not override Element", Name);
}

LX::TypePtr LX::Type::Element(size_t) const
{
    Error("type '{}' does not override Element", Name);
}

size_t LX::Type::IndexOf(const std::string&) const
{
    Error("type '{}' does not override IndexOf", Name);
}
