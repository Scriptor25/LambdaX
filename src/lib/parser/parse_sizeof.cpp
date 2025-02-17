#include <LX/AST.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::ExprPtr LX::Parser::ParseSizeOf()
{
    const auto where = Expect("sizeof").Where;
    Expect(TokenType_ParenOpen);
    const auto type = ParseType();
    Expect(TokenType_ParenClose);
    return std::make_unique<ConstIntExpr>(where, type->Bits / 8);
}
