#include <LX/AST.hpp>
#include <LX/Context.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseSequence()
{
    const auto where = Skip().Where;

    std::vector<ExprPtr> children;
    do
    {
        children.push_back(ParseExpr());
        if (!At(TokenType_ParenClose))
            Expect(TokenType_Comma);
    }
    while (!(At(TokenType_ParenClose) || AtEOF()));
    Expect(TokenType_ParenClose);

    return std::make_unique<SequenceExpr>(where, std::move(children));
}
