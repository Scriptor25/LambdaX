#include <LX/AST.hpp>

LX::FunctionStmt::FunctionStmt(SourceLocation where, Function fun)
    : Stmt(std::move(where)), Fun(std::move(fun))
{
}

std::ostream& LX::FunctionStmt::Print(std::ostream& os) const
{
    return Fun.Print(os);
}

LX::ValuePtr LX::FunctionStmt::GenIR(Builder& builder) const
{
    Where.EmitDI(builder);

    return Fun.GenIR(Where, builder);
}
