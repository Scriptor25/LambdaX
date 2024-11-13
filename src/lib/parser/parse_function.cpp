#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

void LX::Parser::ParseFunction(const SourceLocation&, Function& fun)
{
    Expect("(");
    const auto vararg = ParseParameterList(fun.Params, ")");
    Expect(")");

    TypePtr result;
    if (NextAt("=>"))
        result = ParseType();
    else result = m_Ctx.GetVoidType();

    fun.Type = m_Ctx.GetFunctionType(result, fun.Params, vararg);

    if (NextAt("="))
        fun.Body = ParseExpr();
}
