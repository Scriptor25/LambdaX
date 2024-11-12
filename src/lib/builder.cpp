#include <filesystem>
#include <ranges>
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Scalar/TailRecursionElimination.h>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>
#include <LX/Value.hpp>

LX::ValuePtr& LX::Builder::StackFrame::operator[](const std::string& name)
{
    return Variables[name];
}

const LX::ValuePtr& LX::Builder::StackFrame::operator[](const std::string& name) const
{
    return Variables.at(name);
}

bool LX::Builder::StackFrame::contains(const std::string& name) const
{
    return Variables.contains(name);
}

llvm::DIScope& LX::Builder::StackFrame::DIScope() const
{
    return *Scope;
}

LX::Builder::Builder(Context& ctx, llvm::LLVMContext& context, const std::string& filename)
    : m_Ctx(ctx), m_IRContext(context)
{
    const auto path = std::filesystem::absolute(filename);
    m_Id = path.filename().replace_extension().string();

    m_IRModule = std::make_unique<llvm::Module>(m_Id, IRContext());
    IRModule().setSourceFileName(filename);
    m_IRBuilder = std::make_unique<llvm::IRBuilder<>>(IRContext());

    m_DIBuilder = std::make_unique<llvm::DIBuilder>(IRModule());

    const auto file = DIBuilder().createFile(path.filename().string(), path.parent_path().string());
    m_DIUnit = DIBuilder().createCompileUnit(
        llvm::dwarf::DW_LANG_C,
        file,
        "LambdaX",
        true,
        "",
        0);

    m_FPM = std::make_unique<llvm::FunctionPassManager>();
    m_MPM = std::make_unique<llvm::ModulePassManager>();
    m_LAM = std::make_unique<llvm::LoopAnalysisManager>();
    m_FAM = std::make_unique<llvm::FunctionAnalysisManager>();
    m_CGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
    m_MAM = std::make_unique<llvm::ModuleAnalysisManager>();
    m_PIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
    m_SI = std::make_unique<llvm::StandardInstrumentations>(IRContext(), true);

    m_SI->registerCallbacks(*m_PIC, m_MAM.get());

    m_FPM->addPass(llvm::InstCombinePass());
    m_FPM->addPass(llvm::SimplifyCFGPass());
    m_FPM->addPass(llvm::TailCallElimPass());
    m_FPM->addPass(llvm::VerifierPass());

    m_MPM->addPass(llvm::VerifierPass());

    llvm::PassBuilder pb;
    pb.registerLoopAnalyses(*m_LAM);
    pb.registerFunctionAnalyses(*m_FAM);
    pb.registerCGSCCAnalyses(*m_CGAM);
    pb.registerModuleAnalyses(*m_MAM);
    pb.crossRegisterProxies(*m_LAM, *m_FAM, *m_CGAM, *m_MAM);

    Push();
}

void LX::Builder::Finalize() const
{
    RunPasses(IRModule());
    DIBuilder().finalize();
}

const std::string& LX::Builder::ModuleId() const
{
    return m_Id;
}

LX::Context& LX::Builder::Ctx() const
{
    return m_Ctx;
}

llvm::LLVMContext& LX::Builder::IRContext() const
{
    return m_IRContext;
}

llvm::Module& LX::Builder::IRModule() const
{
    return *m_IRModule;
}

std::unique_ptr<llvm::Module> LX::Builder::IRModulePtr()
{
    return std::move(m_IRModule);
}

llvm::IRBuilder<>& LX::Builder::IRBuilder() const
{
    return *m_IRBuilder;
}

llvm::DIBuilder& LX::Builder::DIBuilder() const
{
    return *m_DIBuilder;
}

llvm::DICompileUnit& LX::Builder::DIUnit() const
{
    return *m_DIUnit;
}

llvm::DIScope& LX::Builder::DIScope() const
{
    return m_Stack.back().DIScope();
}

void LX::Builder::Push(llvm::DIScope* scope)
{
    if (!scope)
        scope = m_Stack.empty()
                    ? nullptr
                    : m_Stack.back().Scope;
    auto& [vars_, scope_] = m_Stack.emplace_back();
    scope_ = scope;
}

void LX::Builder::Pop()
{
    m_Stack.pop_back();
}

LX::ValuePtr& LX::Builder::Define(const SourceLocation& where, const std::string& name)
{
    if (m_Stack.back().contains(name))
        Error(where, "redefining symbol '{}'", name);
    return m_Stack.back()[name];
}

const LX::ValuePtr& LX::Builder::Get(const SourceLocation& where, const std::string& name)
{
    for (const auto& frame : std::ranges::views::reverse(m_Stack))
        if (frame.contains(name)) return frame[name];
    Error(where, "undefined symbol '{}'", name);
}

bool LX::Builder::Contains(const std::string& name)
{
    return std::ranges::any_of(
        std::ranges::views::reverse(m_Stack),
        [&](const auto& frame) { return frame.contains(name); });
}

LX::ValuePtr LX::Builder::CreateCast(const SourceLocation& where, const ValuePtr& src, const TypePtr& dst)
{
    const auto src_ty = src->Type();
    if (src_ty == dst)
        return src;

    const auto src_value = src->Load(*this);
    const auto dst_type = dst->GenIR(*this);

    if (src_ty->IsInt())
    {
        if (dst->IsInt())
        {
            const auto value = IRBuilder().CreateIntCast(src_value, dst_type, dst->IsSigned());
            return RValue::Create(dst, value);
        }
        if (dst->IsFloat())
        {
            llvm::Value* value;
            if (src_ty->IsSigned()) value = IRBuilder().CreateSIToFP(src_value, dst_type);
            else value = IRBuilder().CreateUIToFP(src_value, dst_type);
            return RValue::Create(dst, value);
        }
        if (dst->IsPointer())
        {
            const auto value = IRBuilder().CreateIntToPtr(src_value, dst_type);
            return RValue::Create(dst, value);
        }
    }
    else if (src_ty->IsFloat())
    {
        if (dst->IsInt())
        {
            llvm::Value* value;
            if (dst->IsSigned()) value = IRBuilder().CreateFPToSI(src_value, dst_type);
            else value = IRBuilder().CreateFPToUI(src_value, dst_type);
            return RValue::Create(dst, value);
        }
        if (dst->IsFloat())
        {
            const auto value = IRBuilder().CreateFPCast(src_value, dst_type);
            return RValue::Create(dst, value);
        }
    }
    else if (src_ty->IsPointer())
    {
        if (dst->IsInt())
        {
            const auto value = IRBuilder().CreatePtrToInt(src_value, dst_type);
            return RValue::Create(dst, value);
        }
        if (dst->IsPointer())
        {
            const auto value = src_value;
            return RValue::Create(dst, value);
        }
    }

    Error(where, "cannot cast value of type '{}' to type '{}'", src_ty, dst);
}

LX::ValuePtr LX::Builder::CreateAlloca(const TypePtr& type, const bool is_mutable, const std::string& name)
{
    const auto bkp = IRBuilder().GetInsertBlock();
    IRBuilder().SetInsertPointPastAllocas(bkp->getParent());
    const auto ptr = IRBuilder().CreateAlloca(type->GenIR(*this), {}, name);
    IRBuilder().SetInsertPoint(bkp);
    return LValue::Create(type, ptr, is_mutable);
}

void LX::Builder::Equalize(const SourceLocation& where, ValuePtr& a, ValuePtr& b)
{
    if (a->Type() == b->Type())
        return;

    const auto dst = Type::Equalize(where, m_Ctx, a->Type(), b->Type());
    a = CreateCast(where, a, dst);
    b = CreateCast(where, b, dst);
}

bool LX::Builder::RunPasses(llvm::Function& function) const
{
    return m_FPM->run(function, *m_FAM).areAllPreserved();
}

bool LX::Builder::RunPasses(llvm::Module& module) const
{
    return m_MPM->run(module, *m_MAM).areAllPreserved();
}
