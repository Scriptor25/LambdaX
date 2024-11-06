#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>

bool LX::Parser::ParseParameterList(std::vector<Parameter>& list, const std::string& delim)
{
    while (!(At(delim) || AtEOF()))
    {
        if (NextAt("."))
        {
            Expect(".");
            Expect(".");
            return true;
        }

        ParseParameter(list.emplace_back());
        if (!At(delim))
            Expect(",");
    }
    return false;
}
