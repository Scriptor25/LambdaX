#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseSubscript(ExprPtr ptr)
{
    const auto where = Expect(TokenType_BracketOpen).Where;

    auto index = ParseExpr();
    Expect(TokenType_BracketClose);

    return std::make_unique<SubscriptExpr>(where, std::move(ptr), std::move(index));
}
