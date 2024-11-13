#include <LX/Builder.hpp>
#include <LX/Error.hpp>
#include <LX/Type.hpp>

std::string LX::FloatType::GetName(const unsigned bits)
{
    return 'f' + std::to_string(bits);
}

LX::FloatType::FloatType(const unsigned bits)
    : Type(GetName(bits), bits)
{
}

bool LX::FloatType::IsFloat() const
{
    return true;
}

llvm::Type* LX::FloatType::GenIR(const SourceLocation& where, Builder& builder)
{
    if (m_IR) return m_IR;
    switch (Bits)
    {
    case 16: return m_IR = builder.IRBuilder().getHalfTy();
    case 32: return m_IR = builder.IRBuilder().getFloatTy();
    case 64: return m_IR = builder.IRBuilder().getDoubleTy();
    default: Error(where, "undefined float type with {} bits", Bits);
    }
}

llvm::DIType* LX::FloatType::GenDI(Builder& builder)
{
    if (m_DI) return m_DI;
    return m_DI = builder.DIBuilder().createBasicType(Name, Bits, llvm::dwarf::DW_ATE_float);
}
