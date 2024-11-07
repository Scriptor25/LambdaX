#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Type.hpp>

LX::SymbolExpr::SymbolExpr(TypePtr type, std::string name)
    : Expr(std::move(type)), Name(std::move(name))
{
}

std::ostream& LX::SymbolExpr::Print(std::ostream& os) const
{
    return os << Name;
}

LX::ValuePtr LX::SymbolExpr::GenIR(Builder& builder) const
{
    return builder.GetVar(Name);
}
