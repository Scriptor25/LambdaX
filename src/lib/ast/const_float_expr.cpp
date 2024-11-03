#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ConstFloatExpr::ConstFloatExpr(TypePtr type, const double value)
    : Expr(std::move(type)), Value(value)
{
}

std::ostream& LX::ConstFloatExpr::Print(std::ostream& os) const
{
    return os << Value;
}

void LX::ConstFloatExpr::GenIR(Builder& builder, LX::Value& ref) const
{
    ref.Type = Type;
    ref.TypeIR = Type->GenIR(builder);
    ref.ValueIR = llvm::ConstantFP::get(builder.IRBuilder().getDoubleTy(), Value);
}
