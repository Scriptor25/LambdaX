#include <LX/AST.hpp>
#include <LX/Builder.hpp>

LX::SymbolExpr::SymbolExpr(std::string name)
    : Name(std::move(name))
{
}

std::ostream& LX::SymbolExpr::Print(std::ostream& os) const
{
    return os << Name;
}

void LX::SymbolExpr::GenIR(Builder& builder, Value& ref) const
{
    ref = builder.GetVar(Name);
}
