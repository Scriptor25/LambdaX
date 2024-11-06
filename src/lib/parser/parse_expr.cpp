#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::ExprPtr LX::Parser::ParseExpr()
{
    return ParseBinary(ParseOperand(), 0);
}
