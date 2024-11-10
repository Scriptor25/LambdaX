#include <LX/Builder.hpp>
#include <LX/Type.hpp>

LX::VoidType::VoidType()
    : Type("", 0)
{
}

bool LX::VoidType::IsVoid() const
{
    return true;
}

llvm::Type* LX::VoidType::GenIR(Builder& builder) const
{
    return builder.IRBuilder().getVoidTy();
}
