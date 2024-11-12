#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Linker/Linker.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Linker.hpp>

LX::Linker::Linker(const std::string& module_id)
{
    m_IRContext = std::make_unique<llvm::LLVMContext>();
    m_IRModule = std::make_unique<llvm::Module>(module_id, IRContext());
    m_IRLinker = std::make_unique<llvm::Linker>(IRModule());

    IRModule().setSourceFileName({});
}

llvm::LLVMContext& LX::Linker::IRContext() const
{
    return *m_IRContext;
}

llvm::Module& LX::Linker::IRModule() const
{
    return *m_IRModule;
}

llvm::Linker& LX::Linker::IRLinker() const
{
    return *m_IRLinker;
}

void LX::Linker::Link(Builder& builder) const
{
    const auto name = builder.IRModule().getModuleIdentifier();
    const auto src_filename = builder.IRModule().getSourceFileName();
    if (IRLinker().linkInModule(builder.IRModulePtr()))
        Error("failed to link module '{}'", name);

    const auto dst_filename = IRModule().getSourceFileName();
    IRModule().setSourceFileName(dst_filename.empty() ? src_filename : dst_filename + ',' + src_filename);
}

void LX::Linker::EmitIR(const std::string& filename) const
{
    if (filename.empty())
    {
        IRModule().print(llvm::outs(), nullptr);
        return;
    }

    std::error_code ec;
    llvm::raw_fd_ostream os(filename, ec);
    IRModule().print(os, nullptr);
}

void LX::Linker::EmitObj(const std::string& filename) const
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    const auto triple = llvm::sys::getDefaultTargetTriple();

    std::string err;
    const auto target = llvm::TargetRegistry::lookupTarget(triple, err);

    const auto cpu = "generic";
    const auto features = "";

    const llvm::TargetOptions opt;
    const auto machine = target->createTargetMachine(triple, cpu, features, opt, llvm::Reloc::PIC_);

    m_IRModule->setDataLayout(machine->createDataLayout());
    m_IRModule->setTargetTriple(triple);

    if (filename.empty())
    {
        llvm::legacy::PassManager manager;
        machine->addPassesToEmitFile(manager, llvm::outs(), nullptr, llvm::CodeGenFileType::ObjectFile);
        manager.run(*m_IRModule);
    }
    else
    {
        std::error_code ec;
        llvm::raw_fd_ostream os(filename, ec);
        llvm::legacy::PassManager manager;
        machine->addPassesToEmitFile(manager, os, nullptr, llvm::CodeGenFileType::ObjectFile);
        manager.run(*m_IRModule);
    }
}
