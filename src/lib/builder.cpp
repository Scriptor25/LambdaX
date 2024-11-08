#include <filesystem>
#include <ranges>
#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

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

LX::Builder::Builder(Context& ctx, llvm::LLVMContext& context, const std::string& filename)
    : m_Ctx(ctx), m_IRContext(context)
{
    m_Id = std::filesystem::path(filename).filename().replace_extension().string();

    m_IRModule = std::make_unique<llvm::Module>(m_Id, IRContext());
    m_IRBuilder = std::make_unique<llvm::IRBuilder<>>(IRContext());

    IRModule().setSourceFileName(filename);

    Push();
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

void LX::Builder::Push()
{
    m_Stack.emplace_back();
}

void LX::Builder::Pop()
{
    m_Stack.pop_back();
}

LX::FunctionRef& LX::Builder::GetFunction(const std::string& name)
{
    return m_Functions[m_Id][name];
}

LX::FunctionRef& LX::Builder::GetFunction(const std::string& module_id, const std::string& name)
{
    return m_Functions[module_id][name];
}

LX::ValuePtr& LX::Builder::DefVar(const std::string& name)
{
    if (m_Stack.back().contains(name))
        Error("cannot redefine variable '{}'", name);
    return m_Stack.back()[name];
}

bool LX::Builder::HasVar(const std::string& name)
{
    return std::ranges::any_of(
        std::ranges::views::reverse(m_Stack),
        [&](const auto& frame) { return frame.contains(name); });
}

const LX::ValuePtr& LX::Builder::GetVar(const std::string& name)
{
    for (const auto& frame : std::ranges::views::reverse(m_Stack))
        if (frame.contains(name)) return frame[name];
    Error("undefined variable '{}'", name);
}

LX::ValuePtr LX::Builder::Cast(const ValuePtr& src, const TypePtr& dst)
{
    const auto src_ty = src->Type();
    if (src_ty == dst)
        return src;

    const auto src_value = src->Load(*this);
    const auto dst_type = dst->GetIR(*this);

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

    Error("cannot cast value of type '{}' to type '{}'", src_ty, dst);
}

void LX::Builder::Equalize(ValuePtr& a, ValuePtr& b)
{
    if (a->Type() == b->Type())
        return;

    const auto dst = Type::Equalize(m_Ctx, a->Type(), b->Type());
    a = Cast(a, dst);
    b = Cast(b, dst);
}

llvm::Value* LX::Builder::CreateAlloca(llvm::Type* type, const std::string& name) const
{
    const auto bkp = IRBuilder().GetInsertBlock();
    IRBuilder().SetInsertPointPastAllocas(bkp->getParent());
    const auto ptr = IRBuilder().CreateAlloca(type, {}, name);
    IRBuilder().SetInsertPoint(bkp);
    return ptr;
}
