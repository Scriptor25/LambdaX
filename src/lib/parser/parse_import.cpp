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

        Parse(m_Ctx, stream, path.string(), [&](StmtPtr&& ptr)
        {
            if (const auto f = dynamic_cast<FunctionStmt*>(ptr.get()); f && f->Fun.Export)
            {
                const auto& [
                    src_export_,
                    src_extern_,
                    src_name_,
                    src_type_,
                    src_params_,
                    src_body_
                ] = f->Fun;

                auto& [
                    dst_type_,
                    dst_name_,
                    dst_extern_
                ] = imports.emplace_back();

                dst_type_ = m_Ctx.GetPointerType(src_type_);
                dst_name_ = src_name_;
                dst_extern_ = src_extern_;
            }
        }, true);
        stream.close();

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
