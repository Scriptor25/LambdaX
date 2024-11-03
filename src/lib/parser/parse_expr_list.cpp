#include <LX/AST.hpp>
#include <LX/Parser.hpp>

void LX::Parser::ParseExprList(std::vector<ExprPtr>& list, const std::string& delim)
{
    while (!(At(delim) || AtEOF()))
    {
        list.push_back(ParseExpr());

        if (!At(delim))
            Expect(",");
    }
}
