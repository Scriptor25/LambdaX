#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseSwitch()
{
    const auto where = Expect("switch").Where;
    Expect("(");
    auto switch_ = ParseExpr();
    Expect(")");
    Expect("[");

    std::vector<Case> cases;
    ExprPtr default_;

    while (!(At("]") || AtEOF()))
    {
        if (At("def"))
        {
            const auto def_where = Skip().Where;
            if (default_)
                Error(def_where, "switch expression already has a default case");
            Expect(":");
            default_ = ParseExpr();
        }
        else
        {
            auto& [conditions_, then_] = cases.emplace_back();
            do conditions_.push_back(ParseExpr());
            while (NextAt(","));
            Expect(":");
            then_ = ParseExpr();
        }

        if (!At("]"))
            Expect(",");
        else NextAt(",");
    }
    Expect("]");

    return std::make_unique<SwitchExpr>(where, std::move(switch_), std::move(cases), std::move(default_));
}
