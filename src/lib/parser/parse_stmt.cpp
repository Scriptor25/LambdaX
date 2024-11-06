#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::StmtPtr LX::Parser::ParseStmt()
{
    if (NextAt("type"))
    {
        const auto name = Expect(TokenType_Symbol).StringValue;
        Expect("=");
        const auto type = ParseType();
        m_Ctx.GetType(name) = type;
        return {};
    }

    const auto name = Expect(TokenType_Symbol).StringValue;
    return ParseFunction(name);
}
