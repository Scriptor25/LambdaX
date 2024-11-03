#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>

void LX::Parser::ParseParameterList(std::vector<Parameter>& list, const std::string& delim)
{
    while (!(At(delim) || AtEOF()))
    {
        ParseParameter(list.emplace_back());
        if (!At(delim))
            Expect(",");
    }
}
