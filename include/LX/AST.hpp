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
    };

    struct BinaryExpr : Expr
    {
        BinaryExpr(std::string, ExprPtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        std::string Op;
        ExprPtr Lhs;
        ExprPtr Rhs;
    };

    struct CallExpr : Expr
    {
        CallExpr(ExprPtr, std::vector<ExprPtr>);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        ExprPtr Callee;
        std::vector<ExprPtr> Args;
    };

    struct CastExpr : Expr
    {
        CastExpr(ExprPtr, TypePtr);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        ExprPtr Src;
        TypePtr Dst;
    };

    struct ConstFloatExpr : Expr
    {
        explicit ConstFloatExpr(double);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        double Value;
    };

    struct ConstFunctionExpr : Expr
    {
        ConstFunctionExpr(TypePtr, std::vector<Parameter>, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        TypePtr Type;
        std::vector<Parameter> Params;
        ExprPtr Body;
    };

    struct ConstIntExpr : Expr
    {
        explicit ConstIntExpr(size_t);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        size_t Value;
    };

    struct ConstStringExpr : Expr
    {
        explicit ConstStringExpr(std::string);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        std::string Value;
    };

    struct SequenceExpr : Expr
    {
        explicit SequenceExpr(std::vector<ExprPtr>);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        std::vector<ExprPtr> Children;
    };

    struct SubscriptExpr : Expr
    {
        SubscriptExpr(ExprPtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        ExprPtr Base;
        ExprPtr Offset;
    };

    struct SymbolExpr : Expr
    {
        explicit SymbolExpr(std::string);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        std::string Name;
    };

    struct TernaryExpr : Expr
    {
        TernaryExpr(ExprPtr, ExprPtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        void GenIR(Builder&, Value&) const override;

        ExprPtr Condition;
        ExprPtr Then;
        ExprPtr Else;
    };
}
