#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Value.hpp>

LX::ConstStringExpr::ConstStringExpr(std::string value)
    : Value(std::move(value))
{
}

std::ostream& LX::ConstStringExpr::Print(std::ostream& os) const
{
    return os << Value;
}

void LX::ConstStringExpr::GenIR(Builder& builder, LX::Value& ref) const
{
    ref.Type = builder.Ctx().GetPointerType(builder.Ctx().GetIntType(8, true));
    ref.ValueIR = builder.IRBuilder().CreateGlobalString(Value);
}
