#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

void LX::Parser::ParseFunction(Function& fun)
{
    Expect("(");
    const auto vararg = ParseParameterList(fun.Params, ")");
    Expect(")");

    TypePtr result;
    bool is_mutable = false;
    if (NextAt("=>"))
    {
        is_mutable = NextAt("mut");
        result = ParseType();
    }
    else result = m_Ctx.GetVoidType();

    fun.Type = m_Ctx.GetFunctionType(result, fun.Params, is_mutable, vararg);

    if (NextAt("="))
        fun.Body = ParseExpr();
}
