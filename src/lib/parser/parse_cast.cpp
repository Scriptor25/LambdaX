#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseCast(ExprPtr ptr)
{
    const auto where = Skip().Where;

    const auto type = ParseType();
    return std::make_unique<CastExpr>(where, std::move(ptr), type);
}
