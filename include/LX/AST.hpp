#pragma once

#include <vector>
#include <LX/LX.hpp>
#include <LX/Parameter.hpp>
#include <LX/SourceLocation.hpp>

namespace LX
{
    struct Stmt
    {
        explicit Stmt(SourceLocation);

        virtual ~Stmt() = default;
        virtual std::ostream& Print(std::ostream&) const = 0;
        virtual ValuePtr GenIR(Builder&) const = 0;

        SourceLocation Where;
    };

    struct FunctionStmt : Stmt
    {
        FunctionStmt(
            SourceLocation where,
            bool export_,
            bool extern_,
            TypePtr type,
            std::string name,
            std::vector<Parameter> params,
            ExprPtr body);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        bool Export;
        bool Extern;
        TypePtr Type;
        std::string Name;
        std::vector<Parameter> Params;
        ExprPtr Body;
    };

    struct FunctionImport
    {
        TypePtr Type;
        std::string Name;
        bool Extern;
    };

    struct ImportStmt : Stmt
    {
        ImportStmt(SourceLocation where, std::vector<FunctionImport> imports, std::string module_id, std::string name);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<FunctionImport> Imports;
        std::string ModuleId;
        std::string Name;
    };

    struct Expr : Stmt
    {
        Expr(SourceLocation where, TypePtr type);

        TypePtr Type;
    };

    struct BinaryExpr : Expr
    {
        static TypePtr GetType(
            const SourceLocation& where,
            Context& ctx,
            const std::string& operator_,
            const TypePtr& left,
            const TypePtr& right);

        BinaryExpr(SourceLocation where, TypePtr type, std::string operator_, ExprPtr left, ExprPtr right);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Operator;
        ExprPtr Left;
        ExprPtr Right;
    };

    struct CallExpr : Expr
    {
        CallExpr(SourceLocation where, TypePtr type, ExprPtr callee, std::vector<ExprPtr> args);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Callee;
        std::vector<ExprPtr> Args;
    };

    struct CastExpr : Expr
    {
        CastExpr(SourceLocation where, TypePtr type, ExprPtr source);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Source;
    };

    struct ConstFloatExpr : Expr
    {
        ConstFloatExpr(SourceLocation where, TypePtr type, double value);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        double Value;
    };

    struct ConstFunctionExpr : Expr
    {
        ConstFunctionExpr(SourceLocation where, TypePtr type, std::vector<Parameter> params, ExprPtr body);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<Parameter> Params;
        ExprPtr Body;
    };

    struct ConstIntExpr : Expr
    {
        ConstIntExpr(SourceLocation where, TypePtr type, size_t value);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        size_t Value;
    };

    struct ConstStringExpr : Expr
    {
        ConstStringExpr(SourceLocation where, TypePtr type, std::string value);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Value;
    };

    struct ConstStructExpr : Expr
    {
        ConstStructExpr(SourceLocation where, TypePtr type, std::vector<ExprPtr> elements);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<ExprPtr> Elements;
    };

    struct ImmutableExpr : Expr
    {
        ImmutableExpr(SourceLocation where, TypePtr type, std::string name, ExprPtr init);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Name;
        ExprPtr Init;
    };

    struct MemberExpr : Expr
    {
        MemberExpr(SourceLocation where, TypePtr type, ExprPtr parent, std::string member, bool deref);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Parent;
        std::string Member;
        bool Deref;
    };

    struct MutableExpr : Expr
    {
        MutableExpr(SourceLocation where, TypePtr type, std::string name, ExprPtr init);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Name;
        ExprPtr Init;
    };

    struct SequenceExpr : Expr
    {
        SequenceExpr(SourceLocation where, TypePtr type, std::vector<ExprPtr> children);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<ExprPtr> Children;
    };

    struct SubscriptExpr : Expr
    {
        SubscriptExpr(SourceLocation where, TypePtr type, ExprPtr base, ExprPtr offset);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Base;
        ExprPtr Offset;
    };

    struct SymbolExpr : Expr
    {
        SymbolExpr(SourceLocation where, TypePtr type, std::string name);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Name;
    };

    struct TernaryExpr : Expr
    {
        TernaryExpr(SourceLocation where, TypePtr type, ExprPtr condition, ExprPtr then, ExprPtr else_);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Condition;
        ExprPtr Then;
        ExprPtr Else;
    };

    struct UnaryExpr : Expr
    {
        static TypePtr GetType(
            const SourceLocation& where,
            Context& ctx,
            const std::string& operator_,
            const TypePtr& operand);

        UnaryExpr(SourceLocation where, TypePtr type, std::string operator_, ExprPtr operand);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Operator;
        ExprPtr Operand;
    };
}
