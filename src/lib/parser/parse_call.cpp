#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseCall(ExprPtr ptr)
{
    const auto where = Expect(TokenType_ParenOpen).Where;

    std::vector<ExprPtr> args;
    ParseExprList(args, ")");
    Expect(TokenType_ParenClose);

    return std::make_unique<CallExpr>(where, std::move(ptr), std::move(args));
}
