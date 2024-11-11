#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ConstFloatExpr::ConstFloatExpr(SourceLocation where, const double value)
    : Expr(std::move(where)), Value(value)
{
}

std::ostream& LX::ConstFloatExpr::Print(std::ostream& os) const
{
    return os << Value;
}

LX::ValuePtr LX::ConstFloatExpr::GenIR(Builder& builder) const
{
    Where.EmitDI(builder);

    const auto type = builder.Ctx().GetFloatType(64);
    const auto value = llvm::ConstantFP::get(type->GenIR(builder), Value);
    return RValue::Create(type, value);
}
