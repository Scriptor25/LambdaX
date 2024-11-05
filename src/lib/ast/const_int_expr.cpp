#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Value.hpp>

LX::ConstIntExpr::ConstIntExpr(const size_t value)
    : Value(value)
{
}

std::ostream& LX::ConstIntExpr::Print(std::ostream& os) const
{
    return os << Value;
}

void LX::ConstIntExpr::GenIR(Builder& builder, LX::Value& ref) const
{
    ref.Type = builder.Ctx().GetType("u64");
    ref.ValueIR = builder.IRBuilder().getInt64(Value);
}
