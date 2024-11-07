#include <LX/AST.hpp>

LX::Expr::Expr(TypePtr type)
    : Type(std::move(type))
{
}
