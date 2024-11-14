#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::StmtPtr LX::Parser::ParseGlobal()
{
    const auto where = m_Token.Where;

    Function fun;
    GlobalSymbol global;

    fun.Export = global.Export = NextAt("export");
    fun.Extern = global.Extern = NextAt("extern");
    global.IsMutable = NextAt("mut");
    fun.Name = global.Name = Expect(TokenType_Symbol).StringValue;

    if (!global.IsMutable && At("("))
    {
        ParseFunction(where, fun);
        return std::make_unique<FunctionStmt>(where, std::move(fun));
    }

    if ((fun.Export && (Expect("=>"), true)) || NextAt("=>"))
        global.Type = ParseType();

    if ((!fun.Extern && !(global.IsMutable && global.Type) && (Expect(":="), true)) || NextAt(":="))
        global.Init = ParseExpr();

    return std::make_unique<GlobalStmt>(where, std::move(global));
}
