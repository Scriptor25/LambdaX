#pragma once

#include <format>
#include <string>
#include <vector>
#include <llvm/IR/Type.h>
#include <LX/LX.hpp>

namespace LX
{
    struct Type
    {
        static TypePtr Equalize(Context&, const TypePtr&, const TypePtr&);

        Type(std::string name, unsigned bits);
        virtual ~Type() = default;

        std::ostream& Print(std::ostream&) const;

        [[nodiscard]] virtual bool IsInt() const;
        [[nodiscard]] virtual bool IsSigned() const;
        [[nodiscard]] virtual bool IsFloat() const;
        [[nodiscard]] virtual TypePtr Element() const;
        [[nodiscard]] virtual TypePtr Result() const;
        [[nodiscard]] virtual TypePtr Param(size_t) const;
        [[nodiscard]] virtual llvm::Type* GenIR(Builder&) const = 0;

        std::string Name;
        unsigned Bits;
    };

    struct IntType : Type
    {
        static std::string GetName(unsigned bits, bool sign);

        explicit IntType(unsigned bits, bool sign);

        [[nodiscard]] bool IsInt() const override;
        [[nodiscard]] bool IsSigned() const override;
        [[nodiscard]] llvm::Type* GenIR(Builder&) const override;

        bool Sign;
    };

    struct FloatType : Type
    {
        static std::string GetName(unsigned bits);

        explicit FloatType(unsigned bits);

        [[nodiscard]] bool IsFloat() const override;
        [[nodiscard]] llvm::Type* GenIR(Builder&) const override;
    };

    struct PointerType : Type
    {
        static std::string GetName(const TypePtr& element_type);

        explicit PointerType(TypePtr element_type);

        [[nodiscard]] TypePtr Element() const override;
        [[nodiscard]] llvm::Type* GenIR(Builder&) const override;

        TypePtr ElementType;
    };

    struct FunctionType : Type
    {
        static std::string GetName(const TypePtr& result_type, const std::vector<TypePtr>& param_types);

        explicit FunctionType(TypePtr result_type, std::vector<TypePtr> param_types);

        [[nodiscard]] TypePtr Result() const override;
        [[nodiscard]] TypePtr Param(size_t) const override;
        [[nodiscard]] llvm::Type* GenIR(Builder&) const override;

        TypePtr ResultType;
        std::vector<TypePtr> ParamTypes;
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
