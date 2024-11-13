#include <filesystem>
#include <fstream>
#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

LX::StmtPtr LX::Parser::ParseImport()
{
    const auto where = Expect("import").Where;

    const auto filename = Expect(TokenType_String).StringValue;
    auto path = std::filesystem::path(filename);
    if (path.is_relative())
        path = std::filesystem::path(m_Where.Filename).parent_path() / path;

    std::ifstream stream(path);
    if (!stream)
        Error(where, "failed to open file '{}'", path.string());

    if (const auto as = NextAt("as"); !as || m_IsImported)
    {
        if (as) Expect(TokenType_Symbol);

        Parse(m_Ctx, stream, path.string(), [](StmtPtr&&)
        {
        }, true);

        stream.close();
        return {};
    }

    const auto name = Expect(TokenType_Symbol).StringValue;

    std::vector<ImportPtr> imports;
    {
        auto fi = std::make_unique<FunctionImport>();
        fi->Type = m_Ctx.GetFunPtrType(m_Ctx.GetVoidType(), {}, false);
        fi->Name = "__ctor";
        fi->Extern = false;
        imports.push_back(std::move(fi));
    }
    Parse(m_Ctx, stream, path.string(), [&](StmtPtr&& ptr)
    {
        if (const auto f = dynamic_cast<FunctionStmt*>(ptr.get()); f && f->Fun.Export)
        {
            const auto& [
                src_export_,
                src_extern_,
                src_name_,
                src_params_,
                src_type_,
                src_body_
            ] = f->Fun;

            auto fi = std::make_unique<FunctionImport>();
            fi->Type = m_Ctx.GetPointerType(src_type_);
            fi->Name = src_name_;
            fi->Extern = src_extern_;
            imports.push_back(std::move(fi));
        }
        if (const auto g = dynamic_cast<GlobalStmt*>(ptr.get()); g && g->Global.Export)
        {
            const auto& [
                src_export_,
                src_extern_,
                src_mutable_,
                src_name_,
                src_type_,
                src_init_
            ] = g->Global;

            auto gi = std::make_unique<GlobalImport>();
            gi->Type = m_Ctx.GetReferenceType(src_type_);
            gi->Name = src_name_;
            gi->Extern = src_extern_;
            gi->IsMutable = src_mutable_;
            imports.push_back(std::move(gi));
        }
    }, true);
    stream.close();

    const auto module_id = path.filename().replace_extension().string();
    return std::make_unique<ImportStmt>(where, std::move(imports), module_id, name);
}
