#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseNew()
{
    const auto where = Expect("new").Where;
    Expect(TokenType_ParenOpen);
    const auto is_mutable = NextAt("mut");
    const auto type = ParseType();
    size_t count = 1;
    if (NextAt(TokenType_Comma))
        count = Expect(TokenType_Int).IntegerValue;
    Expect(TokenType_ParenClose);
    return std::make_unique<NewExpr>(where, is_mutable, type, count);
}
