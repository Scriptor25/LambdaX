#include <filesystem>
#include <fstream>
#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::StmtPtr LX::Parser::Parse()
{
    if (NextAt("type"))
    {
        const auto name = Expect(TokenType_Symbol).StringValue;
        if (NextAt("{"))
        {
            std::vector<Parameter> elements;
            ParseParameterList(elements, "}");
            Expect("}");

            m_Ctx.GetType(name) = m_Ctx.GetStructType(name, elements);
            return {};
        }

        Expect("=");
        m_Ctx.GetType(name) = ParseType();
        return {};
    }

    if (At("import"))
        return ParseImport();

    return ParseFunction();
}
