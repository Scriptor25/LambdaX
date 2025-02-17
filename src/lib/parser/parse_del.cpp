#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseDel()
{
    const auto where = Expect("del").Where;
    auto ptr = ParseExpr();
    return std::make_unique<DelExpr>(where, std::move(ptr));
}
