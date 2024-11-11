#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseMut()
{
    const auto where = Skip().Where;

    const auto name = Expect(TokenType_Symbol).StringValue;

    TypePtr type;
    ExprPtr init;

    if (NextAt("=>"))
        type = ParseType();
    if ((!type && (Expect(":="), true)) || NextAt(":="))
        init = ParseExpr();

    return std::make_unique<MutableExpr>(where, name, type, std::move(init));
}
