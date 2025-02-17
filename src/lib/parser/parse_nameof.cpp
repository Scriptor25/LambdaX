#include <LX/AST.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::ExprPtr LX::Parser::ParseNameOf()
{
    const auto where = Expect("nameof").Where;
    Expect(TokenType_ParenOpen);
    const auto type = ParseType();
    Expect(TokenType_ParenClose);
    return std::make_unique<ConstStringExpr>(where, type->Name);
}
