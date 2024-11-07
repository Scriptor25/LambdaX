#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Value.hpp>

LX::ConstFloatExpr::ConstFloatExpr(TypePtr type, const double value)
    : Expr(std::move(type)), Value(value)
{
}

std::ostream& LX::ConstFloatExpr::Print(std::ostream& os) const
{
    return os << Value;
}

LX::ValuePtr LX::ConstFloatExpr::GenIR(Builder& builder) const
{
    const auto value = llvm::ConstantFP::get(builder.IRBuilder().getDoubleTy(), Value);
    return RValue::Create(Type, value);
}
