#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseConstFunction()
{
    const auto where = Skip().Where;

    Function fun;
    ParseFunction(where, fun);

    return std::make_unique<ConstFunctionExpr>(where, std::move(fun));
}
