#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseSubscript(ExprPtr ptr)
{
    const auto where = Expect("[").Where;

    auto index = ParseExpr();
    Expect("]");

    return std::make_unique<SubscriptExpr>(where, std::move(ptr), std::move(index));
}
