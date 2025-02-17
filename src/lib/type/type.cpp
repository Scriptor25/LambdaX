#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

LX::TypePtr LX::Type::Equalize(const SourceLocation& where, Context& ctx, const TypePtr& a, const TypePtr& b)
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

    if (a->IsPointer() && b->IsInt())
        return ctx.GetIntType(a->Bits, b->IsSigned());
    if (a->IsInt() && b->IsPointer())
        return ctx.GetIntType(b->Bits, a->IsSigned());

    Error(where, "cannot equalize types {} and {}", a, b);
}

LX::Type::Type(std::string name, const unsigned bits)
    : Name(std::move(name)), Bits(bits)
{
}

std::ostream& LX::Type::Print(std::ostream& os) const
{
    return os << Name;
}

bool LX::Type::IsVoid() const
{
    return false;
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

bool LX::Type::IsArray() const
{
    return false;
}

bool LX::Type::IsStruct() const
{
    return false;
}

bool LX::Type::IsFunction() const
{
    return false;
}

bool LX::Type::IsReference() const
{
    return false;
}

bool LX::Type::IsMutable() const
{
    return false;
}

LX::TypePtr& LX::Type::Base(const SourceLocation& where)
{
    Error(where, "{} is not a pointer, array or mutable type; cannot get element type", Name);
}

LX::Parameter& LX::Type::Element(const SourceLocation& where, const size_t i)
{
    Error(where, "{} is not a struct type; cannot get element type at {}", Name, i);
}

size_t LX::Type::IndexOf(const SourceLocation& where, const std::string& name)
{
    Error(where, "{} is not a struct type; cannot get index of named element '{}'", Name, name);
}

void LX::Type::SetElements(const SourceLocation& where, const std::vector<Parameter>&)
{
    Error(where, "{} is not a struct type; cannot set elements", Name);
}

LX::TypePtr& LX::Type::Result(const SourceLocation& where)
{
    Error(where, "{} is not a function type; cannot provide result type", Name);
}

LX::Parameter& LX::Type::Param(const SourceLocation& where, const size_t i)
{
    Error(where, "{} is not a function type; cannot provide parameter at {}", Name, i);
}

size_t LX::Type::ParamCount(const SourceLocation& where)
{
    Error(where, "{} is not a function type; cannot provide parameter count", Name);
}

bool LX::Type::HasVarArg(const SourceLocation& where)
{
    Error(where, "{} is not a function type; cannot tell if it has variadic args", Name);
}
