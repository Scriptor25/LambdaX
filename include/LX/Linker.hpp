#pragma once

#include <memory>
#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Linker/Linker.h>
#include <LX/LX.hpp>

namespace LX
{
    class Linker
    {
    public:
        explicit Linker(const std::string&);

        [[nodiscard]] llvm::LLVMContext& IRContext() const;
        [[nodiscard]] llvm::Module& IRModule() const;
        [[nodiscard]] llvm::Linker& IRLinker() const;

        void Link(Builder&) const;
        void EmitIR(const std::string&) const;
        void EmitObj(const std::string&) const;

    private:
        std::unique_ptr<llvm::LLVMContext> m_IRContext;
        std::unique_ptr<llvm::Module> m_IRModule;
        std::unique_ptr<llvm::Linker> m_IRLinker;
    };
}
