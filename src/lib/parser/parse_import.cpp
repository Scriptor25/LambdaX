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

        m_Ctx.Push();
        Parse(m_Ctx, stream, path.string(), [&](StmtPtr&& ptr)
        {
            if (const auto f = dynamic_cast<FunctionStmt*>(ptr.get()); f && f->Export)
            {
                auto& [type_,name_,extern_] = imports.emplace_back();
                type_ = m_Ctx.GetPointerType(f->Type);
                name_ = f->Name;
                extern_ = f->Extern;
                params.emplace_back(type_, name_);
            }
        }, true);
        m_Ctx.Pop();
        stream.close();

        m_Ctx.DefVar(where, name) = m_Ctx.GetStructType(params);

        const auto module_id = path.filename().replace_extension().string();
        return std::make_unique<ImportStmt>(where, std::move(imports), module_id, name);
    }
    if (m_IsImported && NextAt("as"))
        Expect(TokenType_Symbol);

    Parse(m_Ctx, stream, path.string(), [](StmtPtr&&)
    {
    }, true);
    stream.close();

    return {};
}
