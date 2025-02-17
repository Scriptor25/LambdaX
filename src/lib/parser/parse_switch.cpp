#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseSwitch()
{
    const auto where = Expect("switch").Where;
    Expect(TokenType_ParenOpen);
    auto switch_ = ParseExpr();
    Expect(TokenType_ParenClose);
    Expect(TokenType_BracketOpen);

    std::vector<Case> cases;
    ExprPtr default_;

    while (!(At(TokenType_BracketClose) || AtEOF()))
    {
        if (At("def"))
        {
            const auto def_where = Skip().Where;
            if (default_)
                Error(def_where, "switch expression already has a default case");
            Expect(TokenType_Colon);
            default_ = ParseExpr();
        }
        else
        {
            auto& [conditions_, then_] = cases.emplace_back();
            do conditions_.push_back(ParseExpr());
            while (NextAt(TokenType_Comma));
            Expect(TokenType_Colon);
            then_ = ParseExpr();
        }

        if (!At(TokenType_BracketClose))
            Expect(TokenType_Comma);
        else NextAt(TokenType_Comma);
    }
    Expect(TokenType_BracketClose);

    return std::make_unique<SwitchExpr>(where, std::move(switch_), std::move(cases), std::move(default_));
}
