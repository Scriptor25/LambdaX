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

llvm::Type* LX::FloatType::GenIR(Builder& builder) const
{
    switch (Bits)
    {
    case 16: return builder.IRBuilder().getHalfTy();
    case 32: return builder.IRBuilder().getFloatTy();
    case 64: return builder.IRBuilder().getDoubleTy();
    default: Error("undefined float type with {} bits", Bits);
    }
}

llvm::DIType* LX::FloatType::GenDI(Builder& builder) const
{
    return builder.DIBuilder().createBasicType(Name, Bits, llvm::dwarf::DW_ATE_float);
}
