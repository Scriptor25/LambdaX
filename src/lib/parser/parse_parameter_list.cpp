#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>

bool LX::Parser::ParseParameterList(std::vector<Parameter>& list, const TokenType delim)
{
    while (!(At(delim) || AtEOF()))
    {
        if (NextAt(TokenType_Period))
        {
            Expect(TokenType_Period);
            Expect(TokenType_Period);
            return true;
        }

        ParseParameter(list.emplace_back());
        if (!At(delim))
            Expect(TokenType_Comma);
    }
    return false;
}
