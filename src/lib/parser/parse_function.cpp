#include <vector>
#include <LX/AST.hpp>
#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::StmtPtr LX::Parser::ParseFunction(const std::string& name)
{
    Expect("(");
    std::vector<Parameter> params;
    const auto vararg = ParseParameterList(params, ")");
    Expect(")");

    Expect("=>");
    const auto result_type = ParseType();

    std::vector<TypePtr> param_types(params.size());
    for (size_t i = 0; i < params.size(); ++i)
        param_types[i] = params[i].Type;

    auto type = m_Ctx.GetFunctionType(result_type, param_types, vararg);

    ExprPtr body;
    if (NextAt("="))
        body = ParseExpr();

    return std::make_unique<FunctionStmt>(type, name, params, std::move(body));
}
