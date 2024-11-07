#include <filesystem>
#include <fstream>
#include <LX/AST.hpp>
#include <LX/Context.hpp>
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

    if (NextAt("import"))
        return ParseImport();

    if (m_IsImported && !At("export"))
    {
        Skip();
        return {};
    }

    return ParseFunction();
}
