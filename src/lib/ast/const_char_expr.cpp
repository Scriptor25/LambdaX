#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Value.hpp>

LX::ConstCharExpr::ConstCharExpr(SourceLocation where, const char value)
    : Expr(std::move(where)), Value(value)
{
}

std::ostream& LX::ConstCharExpr::Print(std::ostream& os) const
{
    std::string value;
    if (Value >= 0x20) value += Value;
    else
    {
        const auto a = Value / 0x10;
        const auto b = Value % 0x10;

        value += "\\x";
        value += static_cast<char>(a >= 10 ? a - 10 + 'A' : a + '0');
        value += static_cast<char>(b >= 10 ? b - 10 + 'A' : b + '0');
    }
    return os << '\'' << value << '\'';
}

LX::ValuePtr LX::ConstCharExpr::GenIR(Builder& builder) const
{
    const auto type = builder.Ctx().GetIntType(8, true);
    const auto value = builder.IRBuilder().getInt8(Value);
    return RValue::Create(type, value);
}
