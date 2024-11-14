#pragma once

#include <format>
#include <string>
#include <vector>
#include <llvm/IR/DebugInfo.h>
#include <llvm/IR/Type.h>
#include <LX/LX.hpp>
#include <LX/Parameter.hpp>

namespace LX
{
    struct Type
    {
        static TypePtr Equalize(const SourceLocation& where, Context& ctx, const TypePtr& a, const TypePtr& b);

        Type(std::string name, unsigned bits);
        virtual ~Type() = default;

        std::ostream& Print(std::ostream&) const;

        [[nodiscard]] virtual bool IsVoid() const;
        [[nodiscard]] virtual bool IsInt() const;
        [[nodiscard]] virtual bool IsSigned() const;
        [[nodiscard]] virtual bool IsFloat() const;
        [[nodiscard]] virtual bool IsPointer() const;
        [[nodiscard]] virtual bool IsArray() const;
        [[nodiscard]] virtual bool IsStruct() const;
        [[nodiscard]] virtual bool IsFunction() const;
        [[nodiscard]] virtual bool IsReference() const;

        [[nodiscard]] virtual bool IsMutable() const;

        virtual TypePtr& Base(const SourceLocation&);

        virtual Parameter& Element(const SourceLocation&, size_t);
        virtual size_t IndexOf(const SourceLocation&, const std::string&);
        virtual void SetElements(const SourceLocation&, const std::vector<Parameter>&);

        virtual TypePtr& Result(const SourceLocation&);
        virtual Parameter& Param(const SourceLocation&, size_t);
        virtual size_t ParamCount(const SourceLocation&);
        virtual bool HasVarArg(const SourceLocation&);

        virtual llvm::Type* GenIR(const SourceLocation&, Builder&) = 0;
        virtual llvm::DIType* GenDI(Builder&) = 0;

        std::string Name;
        unsigned Bits;

    protected:
        llvm::Type* m_IR{};
        llvm::DIType* m_DI{};
    };

    struct VoidType : Type
    {
        VoidType();

        [[nodiscard]] bool IsVoid() const override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;
    };

    struct IntType : Type
    {
        static std::string GetName(unsigned bits, bool sign);

        IntType(unsigned bits, bool sign);

        [[nodiscard]] bool IsInt() const override;
        [[nodiscard]] bool IsSigned() const override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        bool Sign;
    };

    struct FloatType : Type
    {
        static std::string GetName(unsigned bits);

        explicit FloatType(unsigned bits);

        [[nodiscard]] bool IsFloat() const override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;
    };

    struct PointerType : Type
    {
        static std::string GetName(bool is_mutable, const TypePtr& base_type);

        PointerType(bool is_mutable, TypePtr base_type);

        [[nodiscard]] bool IsPointer() const override;
        [[nodiscard]] bool IsMutable() const override;

        TypePtr& Base(const SourceLocation&) override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        bool Mutable;
        TypePtr BaseType;
    };

    struct ReferenceType : Type
    {
        static std::string GetName(const TypePtr& base_type);

        explicit ReferenceType(TypePtr base_type);

        [[nodiscard]] bool IsReference() const override;

        TypePtr& Base(const SourceLocation&) override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        TypePtr BaseType;
    };

    struct ArrayType : Type
    {
        static std::string GetName(bool is_mutable, const TypePtr& base_type, size_t size);

        ArrayType(bool is_mutable, TypePtr base_type, size_t size);

        [[nodiscard]] bool IsArray() const override;
        [[nodiscard]] bool IsMutable() const override;

        TypePtr& Base(const SourceLocation&) override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        bool Mutable;
        TypePtr BaseType;
        size_t Size;
    };

    struct FunctionType : Type
    {
        static std::string GetName(
            const TypePtr& result_type,
            const std::vector<Parameter>& params,
            bool is_mutable,
            bool vararg);

        FunctionType(TypePtr result_type, std::vector<Parameter> params, bool is_mutable, bool vararg);

        [[nodiscard]] bool IsFunction() const override;
        [[nodiscard]] bool IsMutable() const override;

        TypePtr& Result(const SourceLocation&) override;
        Parameter& Param(const SourceLocation&, size_t) override;
        size_t ParamCount(const SourceLocation&) override;
        bool HasVarArg(const SourceLocation&) override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        TypePtr ResultType;
        std::vector<Parameter> Params;
        bool Mutable;
        bool VarArg;
    };

    struct StructType : Type
    {
        static std::string GetName(const std::string& name, const std::vector<Parameter>& elements);
        static unsigned GetBits(const std::vector<Parameter>& elements);

        StructType(std::string name, std::vector<Parameter> elements);

        [[nodiscard]] bool IsStruct() const override;

        Parameter& Element(const SourceLocation&, size_t) override;
        size_t IndexOf(const SourceLocation&, const std::string&) override;
        void SetElements(const SourceLocation&, const std::vector<Parameter>&) override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        std::string StructName;
        std::vector<Parameter> Elements;
    };
}

template <>
struct std::formatter<LX::TypePtr> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(const LX::TypePtr& type, FormatContext& ctx) const
    {
        return std::formatter<std::string>::format(type ? type->Name : "(null)", ctx);
    }
};
