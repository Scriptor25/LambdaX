#include <LX/AST.hpp>

LX::Stmt::Stmt(SourceLocation where)
    : Where(std::move(where))
{
}
