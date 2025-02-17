#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseConstStruct()
{
    const auto where = Skip().Where;

    std::vector<ExprPtr> elements;
    while (!(At(TokenType_BraceClose) || AtEOF()))
    {
        elements.push_back(ParseExpr());
        if (!At(TokenType_BraceClose))
            Expect(TokenType_Comma);
    }
    Expect(TokenType_BraceClose);
    const auto type = ParseType();
    return std::make_unique<ConstStructExpr>(where, std::move(elements), type);
}
