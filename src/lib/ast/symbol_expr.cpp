#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

LX::SymbolExpr::SymbolExpr(SourceLocation where, std::string name)
    : Expr(std::move(where)), Name(std::move(name))
{
}

std::ostream& LX::SymbolExpr::Print(std::ostream& os) const
{
    return os << Name;
}

LX::ValuePtr LX::SymbolExpr::GenIR(Builder& builder) const
{
    Where.EmitDI(builder);

    return builder.Get(Where, Name);
}
