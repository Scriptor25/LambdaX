#pragma once

#include <map>
#include <string>
#include <vector>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <LX/Context.hpp>
#include <LX/Value.hpp>

namespace LX
{
    class Builder
    {
        struct StackFrame
        {
            Value& operator[](const std::string&);
            const Value& operator[](const std::string&) const;
            [[nodiscard]] bool contains(const std::string&) const;

            std::map<std::string, Value> Variables;
        };

    public:
        Builder(Context&, const std::string&);

        [[nodiscard]] Context& Ctx() const;

        [[nodiscard]] llvm::LLVMContext& IRContext() const;
        [[nodiscard]] llvm::Module& IRModule() const;
        [[nodiscard]] llvm::IRBuilder<>& IRBuilder() const;

        void PrintIR(const std::string&);

        void Push();
        void Pop();

        Value& DefVar(const std::string&);
        const Value& GetVar(const std::string&);

        bool Cast(const Value&, const TypePtr&, Value&);
        bool Equalize(Value&, Value&);

    private:
        Context& m_Ctx;

        std::unique_ptr<llvm::LLVMContext> m_IRContext;
        std::unique_ptr<llvm::Module> m_IRModule;
        std::unique_ptr<llvm::IRBuilder<>> m_IRBuilder;

        std::vector<StackFrame> m_Stack;
    };
}
