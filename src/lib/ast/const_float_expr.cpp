#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Value.hpp>

LX::ConstFloatExpr::ConstFloatExpr(const double value)
    : Value(value)
{
}

std::ostream& LX::ConstFloatExpr::Print(std::ostream& os) const
{
    return os << Value;
}

void LX::ConstFloatExpr::GenIR(Builder& builder, LX::Value& ref) const
{
    ref.Type = builder.Ctx().GetType("f64");
    ref.ValueIR = llvm::ConstantFP::get(builder.IRBuilder().getDoubleTy(), Value);
}
