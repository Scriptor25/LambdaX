#include <LX/Parser.hpp>

void LX::Parser::ParseTypeList(std::vector<TypePtr>& list, const std::string& delim)
{
    while (!(At(delim) || AtEOF()))
    {
        list.push_back(ParseType());
        if (!At(delim))
            Expect(",");
    }
}
