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

llvm::Type* LX::VoidType::GenIR(const SourceLocation&, Builder& builder)
{
    if (m_IR) return m_IR;
    return m_IR = builder.IRBuilder().getVoidTy();
}

llvm::DIType* LX::VoidType::GenDI(Builder& builder)
{
    if (m_DI) return m_DI;
    return m_DI = builder.DIBuilder().createBasicType("void", 0, 0);
}
