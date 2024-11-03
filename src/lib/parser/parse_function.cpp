#include <vector>
#include <LX/AST.hpp>
#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>

LX::StmtPtr LX::Parser::ParseFunction(const std::string& name)
{
    Expect("(");
    std::vector<Parameter> params;
    ParseParameterList(params, ")");
    Expect(")");

    Expect("=>");
    const auto result_type = ParseType();

    std::vector<TypePtr> param_types(params.size());
    for (size_t i = 0; i < params.size(); ++i)
        param_types[i] = params[i].Type;

    auto type = m_Ctx.GetFunctionType(result_type, param_types);
    m_Ctx.DefVar(name) = m_Ctx.GetPointerType(type);

    ExprPtr body;
    if (NextAt("="))
    {
        m_Ctx.Push();
        for (const auto& [type_, name_] : params)
            m_Ctx.DefVar(name_) = type_;
        body = ParseExpr();
        m_Ctx.Pop();
    }

    return std::make_unique<FunctionStmt>(type, name, params, std::move(body));
}
