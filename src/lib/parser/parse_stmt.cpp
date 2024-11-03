#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::StmtPtr LX::Parser::ParseStmt()
{
    const auto name = Expect(TokenType_Symbol).StringValue;

    if (At("(")) return ParseFunction(name);
    return ParseVariable(name);
}
