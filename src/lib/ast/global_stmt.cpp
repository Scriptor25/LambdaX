#include <LX/AST.hpp>
#include <LX/Builder.hpp>

LX::GlobalStmt::GlobalStmt(SourceLocation where, GlobalSymbol global)
    : Stmt(std::move(where)), Global(std::move(global))
{
}

std::ostream& LX::GlobalStmt::Print(std::ostream& os) const
{
    return Global.Print(os);
}

LX::ValuePtr LX::GlobalStmt::GenIR(Builder& builder) const
{
    return Global.GenIR(Where, builder);
}
