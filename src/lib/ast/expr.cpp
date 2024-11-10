#include <LX/AST.hpp>

LX::Expr::Expr(SourceLocation where, TypePtr type)
    : Stmt(std::move(where)), Type(std::move(type))
{
}
