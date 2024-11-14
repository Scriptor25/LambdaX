#pragma once

#include <llvm/IR/Value.h>
#include <LX/LX.hpp>

namespace LX
{
    class Value
    {
    public:
        [[nodiscard]] TypePtr Type() const;

        virtual ~Value() = default;

        virtual llvm::Value* Load(const SourceLocation&, Builder&) const = 0;
        virtual void Store(const SourceLocation&, Builder&, llvm::Value*) const = 0;
        virtual void StoreForce(const SourceLocation&, Builder&, llvm::Value*) const = 0;

        [[nodiscard]] virtual llvm::Value* Ptr(const SourceLocation&) const = 0;
        [[nodiscard]] virtual bool IsMutable() const = 0;
        [[nodiscard]] virtual bool IsReference() const = 0;

    protected:
        explicit Value(TypePtr);

        TypePtr m_Type;
    };

    class RValue : public Value
    {
    public:
        static ValuePtr Create(const TypePtr&, llvm::Value*);

        llvm::Value* Load(const SourceLocation&, Builder&) const override;
        void Store(const SourceLocation&, Builder&, llvm::Value*) const override;
        void StoreForce(const SourceLocation&, Builder&, llvm::Value*) const override;

        [[nodiscard]] llvm::Value* Ptr(const SourceLocation&) const override;
        [[nodiscard]] bool IsMutable() const override;
        [[nodiscard]] bool IsReference() const override;

    private:
        RValue(TypePtr, llvm::Value*);

        llvm::Value* m_Value;
    };

    class LValue : public Value
    {
    public:
        static ValuePtr Create(const TypePtr&, llvm::Value*, bool);

        llvm::Value* Load(const SourceLocation&, Builder&) const override;
        void Store(const SourceLocation&, Builder&, llvm::Value*) const override;
        void StoreForce(const SourceLocation&, Builder&, llvm::Value*) const override;

        [[nodiscard]] llvm::Value* Ptr(const SourceLocation&) const override;
        [[nodiscard]] bool IsMutable() const override;
        [[nodiscard]] bool IsReference() const override;

    private:
        LValue(TypePtr, llvm::Value*, bool);

        llvm::Value* m_Ptr;
        bool m_IsMutable;
    };
}
