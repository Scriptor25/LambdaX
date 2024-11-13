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
        [[nodiscard]] virtual TypePtr Element(const SourceLocation&) const;
        [[nodiscard]] virtual Parameter Element(const SourceLocation&, size_t) const;
        [[nodiscard]] virtual size_t IndexOf(const SourceLocation&, const std::string&) const;
        [[nodiscard]] virtual TypePtr Result(const SourceLocation&) const;
        [[nodiscard]] virtual size_t ParamCount(const SourceLocation&) const;
        [[nodiscard]] virtual TypePtr Param(const SourceLocation&, size_t) const;
        [[nodiscard]] virtual bool HasVarArg(const SourceLocation&) const;
        virtual void PutElements(const SourceLocation&, const std::vector<Parameter>&);

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

        explicit IntType(unsigned bits, bool sign);

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
        static std::string GetName(const TypePtr& element_type);

        explicit PointerType(TypePtr element_type);

        [[nodiscard]] bool IsPointer() const override;
        [[nodiscard]] TypePtr Element(const SourceLocation&) const override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        TypePtr ElementType;
    };

    struct ReferenceType : Type
    {
        static std::string GetName(const TypePtr& element_type);

        explicit ReferenceType(TypePtr element_type);

        [[nodiscard]] bool IsReference() const override;
        [[nodiscard]] TypePtr Element(const SourceLocation&) const override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        TypePtr ElementType;
    };

    struct ArrayType : Type
    {
        static std::string GetName(const TypePtr& element_type, size_t size);

        ArrayType(TypePtr element_type, size_t size);

        [[nodiscard]] bool IsArray() const override;
        [[nodiscard]] TypePtr Element(const SourceLocation&) const override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        TypePtr ElementType;
        size_t Size;
    };

    struct FunctionType : Type
    {
        static std::string GetName(const TypePtr& result_type, const std::vector<Parameter>& params, bool vararg);

        FunctionType(TypePtr result_type, std::vector<Parameter> params, bool vararg);

        [[nodiscard]] bool IsFunction() const override;
        [[nodiscard]] TypePtr Result(const SourceLocation&) const override;
        [[nodiscard]] size_t ParamCount(const SourceLocation&) const override;
        [[nodiscard]] TypePtr Param(const SourceLocation&, size_t) const override;
        [[nodiscard]] bool HasVarArg(const SourceLocation&) const override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        TypePtr ResultType;
        std::vector<Parameter> Params;
        bool VarArg;
    };

    struct StructType : Type
    {
        static std::string GetName(const std::string& name, const std::vector<Parameter>& elements);
        static unsigned GetBits(const std::vector<Parameter>& elements);

        StructType(std::string name, std::vector<Parameter> elements);

        [[nodiscard]] bool IsStruct() const override;
        [[nodiscard]] Parameter Element(const SourceLocation&, size_t) const override;
        [[nodiscard]] size_t IndexOf(const SourceLocation&, const std::string&) const override;
        void PutElements(const SourceLocation&, const std::vector<Parameter>&) override;

        llvm::Type* GenIR(const SourceLocation&, Builder&) override;
        llvm::DIType* GenDI(Builder&) override;

        std::string StructName;
        std::vector<Parameter> Elements;
    };
}

namespace std
{
    template <>
    struct formatter<LX::TypePtr> : formatter<string>
    {
        template <typename FormatContext>
        auto format(const LX::TypePtr& type, FormatContext& ctx) const
        {
            return formatter<string>::format(type ? type->Name : "(null)", ctx);
        }
    };
}
