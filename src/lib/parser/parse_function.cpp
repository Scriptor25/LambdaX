#include <vector>
#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::StmtPtr LX::Parser::ParseFunction()
{
    const bool export_ = NextAt("export");
    const auto name = Expect(TokenType_Symbol).StringValue;

    Expect("(");
    std::vector<Parameter> params;
    const auto vararg = ParseParameterList(params, ")");
    Expect(")");

    Expect("=>");
    const auto result_type = ParseType();

    const auto type = m_Ctx.GetFunctionType(result_type, params, vararg);

    if (!m_IsImported && !m_Ctx.HasVar(name))
        m_Ctx.DefVar(name) = m_Ctx.GetPointerType(type);

    ExprPtr body;
    if (!m_IsImported && NextAt("="))
    {
        m_Ctx.Push();
        for (const auto& [type_, name_] : params)
            m_Ctx.DefVar(name_) = type_;
        body = ParseExpr();
        m_Ctx.Pop();
    }

    return std::make_unique<FunctionStmt>(export_, type, name, params, std::move(body));
}
