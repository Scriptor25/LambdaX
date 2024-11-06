#include <LX/Parser.hpp>

bool LX::Parser::ParseTypeList(std::vector<TypePtr>& list, const std::string& delim)
{
    while (!(At(delim) || AtEOF()))
    {
        if (NextAt("."))
        {
            Expect(".");
            Expect(".");
            return true;
        }

        list.push_back(ParseType());
        if (!At(delim))
            Expect(",");
    }
    return false;
}
