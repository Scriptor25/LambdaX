#pragma once

#include <map>
#include <string>
#include <vector>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/StandardInstrumentations.h>
#include <LX/LX.hpp>

namespace LX
{
    class Builder
    {
        struct StackFrame
        {
            ValuePtr& operator[](const std::string&);
            const ValuePtr& operator[](const std::string&) const;
            [[nodiscard]] bool contains(const std::string&) const;

            [[nodiscard]] llvm::DIScope& DIScope() const;

            std::map<std::string, ValuePtr> Variables;
            llvm::DIScope* Scope;
        };

    public:
        Builder(Context&, llvm::LLVMContext&, const std::string&);
        void Finalize() const;

        [[nodiscard]] const std::string& ModuleId() const;

        [[nodiscard]] Context& Ctx() const;

        [[nodiscard]] llvm::LLVMContext& IRContext() const;
        [[nodiscard]] llvm::Module& IRModule() const;
        [[nodiscard]] std::unique_ptr<llvm::Module> IRModulePtr();
        [[nodiscard]] llvm::IRBuilder<>& IRBuilder() const;

        [[nodiscard]] llvm::DIBuilder& DIBuilder() const;
        [[nodiscard]] llvm::DICompileUnit& DIUnit() const;
        [[nodiscard]] llvm::DIScope& DIScope() const;

        void Push(llvm::DIScope* = {});
        void Pop();

        ValuePtr& Define(const SourceLocation& where, const std::string& name);
        const ValuePtr& Get(const SourceLocation& where, const std::string& name);
        bool Contains(const std::string&);

        ValuePtr Cast(const SourceLocation& where, const ValuePtr& src, const TypePtr& dst);
        void Equalize(const SourceLocation& where, ValuePtr& a, ValuePtr& b);

        llvm::Value* CreateAlloca(llvm::Type*, const std::string& = {}) const;

        bool RunPasses(llvm::Function&) const;
        bool RunPasses(llvm::Module&) const;

    private:
        std::string m_Id;

        Context& m_Ctx;
        llvm::LLVMContext& m_IRContext;

        std::unique_ptr<llvm::Module> m_IRModule;
        std::unique_ptr<llvm::IRBuilder<>> m_IRBuilder;

        std::unique_ptr<llvm::DIBuilder> m_DIBuilder;
        llvm::DICompileUnit* m_DIUnit;

        std::unique_ptr<llvm::FunctionPassManager> m_FPM;
        std::unique_ptr<llvm::ModulePassManager> m_MPM;
        std::unique_ptr<llvm::LoopAnalysisManager> m_LAM;
        std::unique_ptr<llvm::FunctionAnalysisManager> m_FAM;
        std::unique_ptr<llvm::CGSCCAnalysisManager> m_CGAM;
        std::unique_ptr<llvm::ModuleAnalysisManager> m_MAM;
        std::unique_ptr<llvm::PassInstrumentationCallbacks> m_PIC;
        std::unique_ptr<llvm::StandardInstrumentations> m_SI;

        std::vector<StackFrame> m_Stack;
    };
}
