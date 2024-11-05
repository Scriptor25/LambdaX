#include <filesystem>
#include <ranges>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

LX::Value& LX::Builder::StackFrame::operator[](const std::string& name)
{
    return Variables[name];
}

const LX::Value& LX::Builder::StackFrame::operator[](const std::string& name) const
{
    return Variables.at(name);
}

bool LX::Builder::StackFrame::contains(const std::string& name) const
{
    return Variables.contains(name);
}

LX::Builder::Builder(Context& ctx, llvm::LLVMContext& context, const std::string& filename)
    : m_Ctx(ctx), m_IRContext(context)
{
    const auto module_id = std::filesystem::path(filename).filename().string();

    m_IRModule = std::make_unique<llvm::Module>(module_id, IRContext());
    m_IRBuilder = std::make_unique<llvm::IRBuilder<>>(IRContext());

    IRModule().setSourceFileName(filename);

    Push();
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

void LX::Builder::Push()
{
    m_Stack.emplace_back();
}

void LX::Builder::Pop()
{
    m_Stack.pop_back();
}

LX::Value& LX::Builder::DefVar(const std::string& name)
{
    if (m_Stack.back().contains(name))
        Error("cannot redefine variable '{}'", name);
    return m_Stack.back()[name];
}

const LX::Value& LX::Builder::GetVar(const std::string& name)
{
    for (const auto& frame : std::ranges::views::reverse(m_Stack))
        if (frame.contains(name)) return frame[name];
    Error("undefined variable '{}'", name);
}

void LX::Builder::Cast(const Value& src, const TypePtr& dst_ty, Value& dst)
{
    if (!src) Error("cannot cast null value");
    if (!dst_ty) Error("cannot cast to null type");

    const auto src_ty = src.Type;
    if (src_ty == dst_ty)
    {
        dst = src;
        return;
    }

    const auto src_val_ir = src.ValueIR;
    const auto dst_ty_ir = dst_ty->GenIR(*this);
    dst.Type = dst_ty;

    if (src_ty->IsInt())
    {
        if (dst_ty->IsInt())
        {
            dst.ValueIR = IRBuilder().CreateIntCast(src_val_ir, dst_ty_ir, dst_ty->IsSigned());
            return;
        }
        if (dst_ty->IsFloat())
        {
            if (src_ty->IsSigned()) dst.ValueIR = IRBuilder().CreateSIToFP(src_val_ir, dst_ty_ir);
            else dst.ValueIR = IRBuilder().CreateUIToFP(src_val_ir, dst_ty_ir);
            return;
        }
        if (dst_ty->IsPointer())
        {
            dst.ValueIR = IRBuilder().CreateIntToPtr(src_val_ir, dst_ty_ir);
            return;
        }
    }
    else if (src_ty->IsFloat())
    {
        if (dst_ty->IsInt())
        {
            if (dst_ty->IsSigned()) dst.ValueIR = IRBuilder().CreateFPToSI(src_val_ir, dst_ty_ir);
            else dst.ValueIR = IRBuilder().CreateFPToUI(src_val_ir, dst_ty_ir);
            return;
        }
        if (dst_ty->IsFloat())
        {
            dst.ValueIR = IRBuilder().CreateFPCast(src_val_ir, dst_ty_ir);
            return;
        }
    }
    else if (src_ty->IsPointer())
    {
        if (dst_ty->IsInt())
        {
            dst.ValueIR = IRBuilder().CreatePtrToInt(src_val_ir, dst_ty_ir);
            return;
        }
        if (dst_ty->IsPointer())
        {
            dst.ValueIR = src.ValueIR;
            return;
        }
    }

    Error("cannot cast value of type '{}' to type '{}'", src_ty, dst_ty);
}

void LX::Builder::Equalize(Value& a, Value& b)
{
    if (!a || !b)
        Error("cannot equalize if one or more values are missing");

    if (a.Type == b.Type)
        return;

    const auto dst_ty = Type::Equalize(m_Ctx, a.Type, b.Type);
    Cast(a, dst_ty, a);
    Cast(b, dst_ty, b);
}
