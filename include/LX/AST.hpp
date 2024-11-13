#pragma once

#include <vector>
#include <llvm/IR/Constant.h>
#include <LX/LX.hpp>
#include <LX/Parameter.hpp>
#include <LX/SourceLocation.hpp>

namespace LX
{
    struct Function
    {
        std::ostream& Print(std::ostream&) const;
        ValuePtr GenIR(const SourceLocation&, Builder&) const;

        bool Export = false;
        bool Extern = false;
        std::string Name;
        std::vector<Parameter> Params;
        TypePtr Type;
        ExprPtr Body;
    };

    struct GlobalSymbol
    {
        std::ostream& Print(std::ostream&) const;
        ValuePtr GenIR(const SourceLocation&, Builder&) const;

        bool Export = false;
        bool Extern = false;
        bool IsMutable = false;
        std::string Name;
        TypePtr Type;
        ExprPtr Init;
    };

    struct Import
    {
        virtual ~Import() = default;
        [[nodiscard]] virtual llvm::Constant* Resolve(const SourceLocation&, Builder&, const std::string&) const = 0;

        TypePtr Type;
        std::string Name;
        bool Extern;
        bool IsMutable = false;
    };

    typedef std::unique_ptr<Import> ImportPtr;

    struct FunctionImport : Import
    {
        [[nodiscard]] llvm::Constant* Resolve(const SourceLocation&, Builder&, const std::string&) const override;
    };

    struct GlobalImport : Import
    {
        [[nodiscard]] llvm::Constant* Resolve(const SourceLocation&, Builder&, const std::string&) const override;
    };

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
        FunctionStmt(SourceLocation where, Function fun);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        Function Fun;
    };

    struct GlobalStmt : Stmt
    {
        GlobalStmt(SourceLocation where, GlobalSymbol global);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        GlobalSymbol Global;
    };

    struct ImportStmt : Stmt
    {
        ImportStmt(SourceLocation where, std::vector<ImportPtr> imports, std::string module_id, std::string name);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<ImportPtr> Imports;
        std::string ModuleId;
        std::string Name;
    };

    struct Expr : Stmt
    {
        explicit Expr(SourceLocation where);
    };

    struct BinaryExpr : Expr
    {
        static TypePtr GetType(
            const SourceLocation& where,
            Context& ctx,
            const std::string& operator_,
            const TypePtr& left,
            const TypePtr& right);

        BinaryExpr(SourceLocation where, std::string operator_, ExprPtr left, ExprPtr right);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Operator;
        ExprPtr Left;
        ExprPtr Right;
    };

    struct CallExpr : Expr
    {
        CallExpr(SourceLocation where, ExprPtr callee, std::vector<ExprPtr> args);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Callee;
        std::vector<ExprPtr> Args;
    };

    struct CastExpr : Expr
    {
        CastExpr(SourceLocation where, ExprPtr source, TypePtr dest);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Source;
        TypePtr Dest;
    };

    struct ConstFloatExpr : Expr
    {
        ConstFloatExpr(SourceLocation where, double value);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        double Value;
    };

    struct ConstFunctionExpr : Expr
    {
        ConstFunctionExpr(SourceLocation where, Function fun);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        Function Fun;
    };

    struct ConstIntExpr : Expr
    {
        ConstIntExpr(SourceLocation where, size_t value);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        size_t Value;
    };

    struct ConstStringExpr : Expr
    {
        ConstStringExpr(SourceLocation where, std::string value);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Value;
    };

    struct ConstStructExpr : Expr
    {
        ConstStructExpr(SourceLocation where, std::vector<ExprPtr> elements, TypePtr type);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<ExprPtr> Elements;
        TypePtr Type;
    };

    struct DefineExpr : Expr
    {
        DefineExpr(SourceLocation where, bool is_mutable, std::string name, TypePtr type, ExprPtr init);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        bool IsMutable;
        std::string Name;
        TypePtr Type;
        ExprPtr Init;
    };

    struct MemberExpr : Expr
    {
        MemberExpr(SourceLocation where, ExprPtr parent, std::string member, bool deref);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Parent;
        std::string Member;
        bool Deref;
    };

    struct SequenceExpr : Expr
    {
        SequenceExpr(SourceLocation where, std::vector<ExprPtr> children);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::vector<ExprPtr> Children;
    };

    struct SubscriptExpr : Expr
    {
        SubscriptExpr(SourceLocation where, ExprPtr base, ExprPtr offset);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        ExprPtr Base;
        ExprPtr Offset;
    };

    struct SymbolExpr : Expr
    {
        SymbolExpr(SourceLocation where, std::string name);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Name;
    };

    struct TernaryExpr : Expr
    {
        TernaryExpr(SourceLocation where, ExprPtr condition, ExprPtr then, ExprPtr else_);

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

        UnaryExpr(SourceLocation where, std::string operator_, ExprPtr operand);

        std::ostream& Print(std::ostream&) const override;
        ValuePtr GenIR(Builder&) const override;

        std::string Operator;
        ExprPtr Operand;
    };
}
