#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseMut()
{
    const auto where = Expect("mut").Where;
    const auto name = Expect(TokenType_Symbol).StringValue;

    TypePtr type;
    ExprPtr init;

    if (NextAt("=>"))
        type = ParseType();
    if ((!type && (Expect(":="), true)) || NextAt(":="))
        init = ParseExpr();

    return std::make_unique<DefineExpr>(where, true, name, type, std::move(init));
}
