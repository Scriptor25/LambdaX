#include <filesystem>
#include <fstream>
#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

LX::StmtPtr LX::Parser::ParseImport()
{
    const auto filename = Expect(TokenType_String).StringValue;
    auto path = std::filesystem::path(filename);
    if (path.is_relative())
        path = std::filesystem::path(m_Where.Filename).parent_path() / path;

    std::ifstream stream(path);

    if (!m_IsImported && NextAt("as"))
    {
        const auto name = Expect(TokenType_Symbol).StringValue;

        std::vector<Parameter> imports;
        Parse(m_Ctx, stream, path.string(), [&](StmtPtr&& ptr)
        {
            if (const auto fun = dynamic_cast<FunctionStmt*>(ptr.get()); fun && fun->Export)
                imports.emplace_back(m_Ctx.GetPointerType(fun->Type), fun->Name);
        }, true);
        stream.close();

        m_Ctx.DefVar(name) = m_Ctx.GetStructType(imports);
        return std::make_unique<ImportStmt>(imports, name);
    }

    if (m_IsImported && NextAt("as"))
        Expect(TokenType_Symbol);

    Parse(m_Ctx, stream, path.string(), [](StmtPtr&&)
    {
    }, true);
    stream.close();

    return {};
}
