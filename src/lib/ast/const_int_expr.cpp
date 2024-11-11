#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Value.hpp>

LX::ConstIntExpr::ConstIntExpr(SourceLocation where, const size_t value)
    : Expr(std::move(where)), Value(value)
{
}

std::ostream& LX::ConstIntExpr::Print(std::ostream& os) const
{
    return os << Value;
}

LX::ValuePtr LX::ConstIntExpr::GenIR(Builder& builder) const
{
    Where.EmitDI(builder);

    const auto type = builder.Ctx().GetIntType(64, false);
    const auto value = builder.IRBuilder().getInt64(Value);
    return RValue::Create(type, value);
}
