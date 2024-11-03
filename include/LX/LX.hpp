#pragma once

#include <memory>

namespace LX
{
    struct SourceLocation;
    struct Token;

    class Context;
    class Parser;
    class Builder;

    struct Type;
    typedef std::shared_ptr<Type> TypePtr;

    struct Parameter;

    struct Stmt;
    typedef std::unique_ptr<Stmt> StmtPtr;

    struct Expr;
    typedef std::unique_ptr<Expr> ExprPtr;

    struct Value;
}
