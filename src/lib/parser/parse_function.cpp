#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

void LX::Parser::ParseFunction(const SourceLocation& where, Function& fun)
{
    bool vararg = false;
    if (NextAt("("))
    {
        vararg = ParseParameterList(fun.Params, ")");
        Expect(")");
    }

    TypePtr result;
    if (NextAt("=>"))
        result = ParseType();
    else result = m_Ctx.GetVoidType();

    fun.Type = m_Ctx.GetFunctionType(result, fun.Params, vararg);

    if (NextAt("="))
        fun.Body = ParseExpr();
}

LX::StmtPtr LX::Parser::ParseFunction()
{
    const auto where = m_Token.Where;

    Function fun;
    fun.Export = NextAt("export");
    fun.Extern = NextAt("extern");
    fun.Name = Expect(TokenType_Symbol).StringValue;
    ParseFunction(where, fun);

    return std::make_unique<FunctionStmt>(where, std::move(fun));
}
