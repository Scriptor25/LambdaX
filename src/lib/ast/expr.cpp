#include <LX/AST.hpp>

LX::Expr::Expr(SourceLocation where)
    : Stmt(std::move(where))
{
}
