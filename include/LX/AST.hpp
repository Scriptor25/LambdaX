#pragma once

#include <vector>
#include <LX/LX.hpp>
#include <LX/Parameter.hpp>

namespace LX
{
    struct Stmt
    {
        virtual ~Stmt() = default;
        virtual std::ostream& Print(std::ostream&) const = 0;
        virtual void GenIR(Builder&, Value&) const = 0;
    };

    struct FunctionStmt : Stmt
    {
        FunctionStmt(TypePtr, std::string, std::vector<Parameter>, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        TypePtr Type;
        std::string Name;
        std::vector<Parameter> Params;
        ExprPtr Body;
    };

    struct Expr : Stmt
    {
        explicit Expr(TypePtr);

        TypePtr Type;
    };

    struct BinaryExpr : Expr
    {
        BinaryExpr(TypePtr, std::string, ExprPtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        std::string Op;
        ExprPtr Lhs;
        ExprPtr Rhs;
    };

    struct CallExpr : Expr
    {
        CallExpr(TypePtr, ExprPtr, std::vector<ExprPtr>);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        ExprPtr Callee;
        std::vector<ExprPtr> Args;
    };

    struct ConstFloatExpr : Expr
    {
        explicit ConstFloatExpr(TypePtr, double);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        double Value;
    };

    struct ConstIntExpr : Expr
    {
        explicit ConstIntExpr(TypePtr, size_t);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        size_t Value;
    };

    struct SymbolExpr : Expr
    {
        explicit SymbolExpr(TypePtr, std::string);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        std::string Name;
    };

    struct TernaryExpr : Expr
    {
        TernaryExpr(TypePtr, ExprPtr, ExprPtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        ExprPtr Condition;
        ExprPtr Then;
        ExprPtr Else;
    };
}
