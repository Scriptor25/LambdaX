#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseSymbol()
{
    const auto [
        where_,
        type_,
        name_,
        int_,
        float_
    ] = Skip();

    TypePtr type;
    ExprPtr init;

    if (NextAt("=>"))
        type = ParseType();
    if ((type && (Expect(":="), true)) || NextAt(":="))
        init = ParseExpr();

    if (!init)
        return std::make_unique<SymbolExpr>(where_, name_);

    return std::make_unique<DefineExpr>(where_, false, name_, type, std::move(init));
}
