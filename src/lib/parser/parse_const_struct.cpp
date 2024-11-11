#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseConstStruct()
{
    const auto where = Skip().Where;

    std::vector<ExprPtr> elements;
    while (!(At("}") || AtEOF()))
    {
        elements.push_back(ParseExpr());
        if (!At("}"))
            Expect(",");
    }
    Expect("}");

    TypePtr type;
    if (NextAt("=>"))
        type = ParseType();

    return std::make_unique<ConstStructExpr>(where, std::move(elements), type);
}
