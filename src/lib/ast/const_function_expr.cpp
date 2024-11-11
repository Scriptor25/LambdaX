#include <LX/AST.hpp>

LX::ConstFunctionExpr::ConstFunctionExpr(SourceLocation where, Function fun)
    : Expr(std::move(where)), Fun(std::move(fun))
{
}

std::ostream& LX::ConstFunctionExpr::Print(std::ostream& os) const
{
    return Fun.Print(os);
}

LX::ValuePtr LX::ConstFunctionExpr::GenIR(Builder& builder) const
{
    Where.EmitDI(builder);

    return Fun.GenIR(Where, builder);
}
