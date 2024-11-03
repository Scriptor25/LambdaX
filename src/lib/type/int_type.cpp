#include <LX/Builder.hpp>
#include <LX/Type.hpp>

std::string LX::IntType::GetName(const unsigned bits, const bool sign)
{
    return (sign ? 'i' : 'u') + std::to_string(bits);
}

LX::IntType::IntType(const unsigned bits, const bool sign)
    : Type(GetName(bits, sign), bits), Sign(sign)
{
}

bool LX::IntType::IsInt() const
{
    return true;
}

bool LX::IntType::IsSigned() const
{
    return Sign;
}

llvm::Type* LX::IntType::GenIR(Builder& builder) const
{
    return builder.IRBuilder().getIntNTy(Bits);
}
