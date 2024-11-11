#include <llvm/Linker/Linker.h>
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

void LX::Linker::Output(const std::string& filename) const
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
