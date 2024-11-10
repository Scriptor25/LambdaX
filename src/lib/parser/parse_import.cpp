#include <filesystem>
#include <fstream>
#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

LX::StmtPtr LX::Parser::ParseImport()
{
    const auto where = Expect("import").Where;

    const auto filename = Expect(TokenType_String).StringValue;
    auto path = std::filesystem::path(filename);
    if (path.is_relative())
        path = std::filesystem::path(m_Where.Filename).parent_path() / path;

    std::ifstream stream(path);

    if (!m_IsImported && NextAt("as"))
    {
        const auto name = Expect(TokenType_Symbol).StringValue;

        std::vector<FunctionImport> imports;
        std::vector<Parameter> params;

        Parse(m_Ctx, stream, path.string(), [&](StmtPtr&& ptr)
        {
            if (const auto fun = dynamic_cast<FunctionStmt*>(ptr.get()); fun && fun->Export)
            {
                auto& [type_, name_, extern_] = imports.emplace_back();
                type_ = m_Ctx.GetPointerType(fun->Type);
                name_ = fun->Name;
                extern_ = fun->Extern;
                params.emplace_back(type_, name_);
            }
        }, true);
        stream.close();

        m_Ctx.DefVar(where, name) = m_Ctx.GetStructType(params);

        const auto module_id = path.filename().replace_extension().string();
        return std::make_unique<ImportStmt>(where, imports, module_id, name);
    }

    if (m_IsImported && NextAt("as"))
        Expect(TokenType_Symbol);

    Parse(m_Ctx, stream, path.string(), [](StmtPtr&&)
    {
    }, true);
    stream.close();

    return {};
}
