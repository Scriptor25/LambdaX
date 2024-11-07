#pragma once

#include <map>
#include <string>
#include <vector>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <LX/Value.hpp>

namespace LX
{
    class Builder
    {
        struct StackFrame
        {
            ValuePtr& operator[](const std::string&);
            const ValuePtr& operator[](const std::string&) const;
            [[nodiscard]] bool contains(const std::string&) const;

            std::map<std::string, ValuePtr> Variables;
        };

    public:
        Builder(Context&, llvm::LLVMContext&, const std::string&);

        [[nodiscard]] Context& Ctx() const;

        [[nodiscard]] llvm::LLVMContext& IRContext() const;
        [[nodiscard]] llvm::Module& IRModule() const;
        [[nodiscard]] std::unique_ptr<llvm::Module> IRModulePtr();
        [[nodiscard]] llvm::IRBuilder<>& IRBuilder() const;

        void Push();
        void Pop();

        ValuePtr& DefVar(const std::string&);
        bool HasVar(const std::string&);
        const ValuePtr& GetVar(const std::string&);

        ValuePtr Cast(const ValuePtr&, const TypePtr&);
        void Equalize(ValuePtr&, ValuePtr&);

        llvm::Value* CreateAlloca(llvm::Type*, const std::string& = {}) const;

    private:
        Context& m_Ctx;
        llvm::LLVMContext& m_IRContext;

        std::unique_ptr<llvm::Module> m_IRModule;
        std::unique_ptr<llvm::IRBuilder<>> m_IRBuilder;

        std::vector<StackFrame> m_Stack;
    };
}
