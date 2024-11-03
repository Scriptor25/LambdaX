#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ConstIntExpr::ConstIntExpr(TypePtr type, const size_t value)
    : Expr(std::move(type)), Value(value)
{
}

std::ostream& LX::ConstIntExpr::Print(std::ostream& os) const
{
    return os << Value;
}

void LX::ConstIntExpr::GenIR(Builder& builder, LX::Value& ref) const
{
    ref.Type = Type;
    ref.TypeIR = Type->GenIR(builder);
    ref.ValueIR = builder.IRBuilder().getInt64(Value);
}
