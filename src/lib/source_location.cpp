#include <LX/Builder.hpp>
#include <LX/SourceLocation.hpp>

void LX::SourceLocation::EmitDI(const Builder& builder) const
{
    const auto scope = &builder.DIScope();
    builder.IRBuilder().SetCurrentDebugLocation(llvm::DILocation::get(builder.IRContext(), Row, Col, scope));
}
