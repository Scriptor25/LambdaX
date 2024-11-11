#pragma once

#include <memory>

namespace LX
{
    struct SourceLocation;
    struct Token;

    class Context;
    class Parser;
    class Builder;
    class Linker;

    struct Type;
    typedef std::shared_ptr<Type> TypePtr;

    struct Parameter;
    struct Function;
    struct FunctionImport;

    struct Stmt;
    typedef std::unique_ptr<Stmt> StmtPtr;

    struct Expr;
    typedef std::unique_ptr<Expr> ExprPtr;

    class Value;
    typedef std::shared_ptr<Value> ValuePtr;
}
