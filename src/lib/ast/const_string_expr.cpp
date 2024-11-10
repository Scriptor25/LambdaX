#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Value.hpp>

LX::ConstStringExpr::ConstStringExpr(SourceLocation where, TypePtr type, std::string value)
    : Expr(std::move(where), std::move(type)), Value(std::move(value))
{
}

std::ostream& LX::ConstStringExpr::Print(std::ostream& os) const
{
    std::string value;
    for (const auto& c : Value)
    {
        if (c >= 0x20) value += c;
        else
        {
            const auto a = c / 0x10;
            const auto b = c % 0x10;

            value += "\\x";
            value += static_cast<char>(a >= 10 ? a - 10 + 'A' : a + '0');
            value += static_cast<char>(b >= 10 ? b - 10 + 'A' : b + '0');
        }
    }
    return os << '"' << value << '"';
}

LX::ValuePtr LX::ConstStringExpr::GenIR(Builder& builder) const
{
    const auto value = builder.IRBuilder().CreateGlobalString(Value);
    return RValue::Create(Type, value);
}
