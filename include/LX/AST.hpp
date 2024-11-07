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
        virtual ValuePtr GenIR(Builder&) const = 0;
    };

    struct FunctionStmt : Stmt
    {
        FunctionStmt(bool, TypePtr, std::string, std::vector<Parameter>, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        bool Export;
        TypePtr Type;
        std::string Name;
        std::vector<Parameter> Params;
        ExprPtr Body;
    };

    struct ImportStmt : Stmt
    {
        ImportStmt(std::vector<Parameter>, std::string);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<Parameter> Imports;
        std::string Name;
    };

    struct Expr : Stmt
    {
        explicit Expr(TypePtr);

        TypePtr Type;
    };

    struct BinaryExpr : Expr
    {
        static TypePtr GetType(Context&, const std::string&, const TypePtr&, const TypePtr&);

        BinaryExpr(TypePtr, std::string, ExprPtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Op;
        ExprPtr Lhs;
        ExprPtr Rhs;
    };

    struct CallExpr : Expr
    {
        CallExpr(TypePtr, ExprPtr, std::vector<ExprPtr>);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Callee;
        std::vector<ExprPtr> Args;
    };

    struct CastExpr : Expr
    {
        CastExpr(TypePtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Src;
    };

    struct ConstFloatExpr : Expr
    {
        ConstFloatExpr(TypePtr, double);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        double Value;
    };

    struct ConstFunctionExpr : Expr
    {
        ConstFunctionExpr(TypePtr, std::vector<Parameter>, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<Parameter> Params;
        ExprPtr Body;
    };

    struct ConstIntExpr : Expr
    {
        ConstIntExpr(TypePtr, size_t);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        size_t Value;
    };

    struct ConstStringExpr : Expr
    {
        ConstStringExpr(TypePtr, std::string);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Value;
    };

    struct ConstStructExpr : Expr
    {
        ConstStructExpr(TypePtr, std::vector<ExprPtr>);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<ExprPtr> Elements;
    };

    struct ImmutableExpr : Expr
    {
        ImmutableExpr(std::string, TypePtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Name;
        ExprPtr Init;
    };

    struct MemberExpr : Expr
    {
        MemberExpr(TypePtr, ExprPtr, std::string);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Parent;
        std::string Member;
    };

    struct MutableExpr : Expr
    {
        MutableExpr(std::string, TypePtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Name;
        ExprPtr Init;
    };

    struct SequenceExpr : Expr
    {
        SequenceExpr(TypePtr, std::vector<ExprPtr>);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<ExprPtr> Children;
    };

    struct SubscriptExpr : Expr
    {
        SubscriptExpr(TypePtr, ExprPtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Base;
        ExprPtr Offset;
    };

    struct SymbolExpr : Expr
    {
        SymbolExpr(TypePtr, std::string);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Name;
    };

    struct TernaryExpr : Expr
    {
        TernaryExpr(TypePtr, ExprPtr, ExprPtr, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Condition;
        ExprPtr Then;
        ExprPtr Else;
    };

    struct UnaryExpr : Expr
    {
        static TypePtr GetType(Context&, const std::string&, const TypePtr&);

        UnaryExpr(TypePtr, std::string, ExprPtr);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Operator;
        ExprPtr Operand;
    };
}
