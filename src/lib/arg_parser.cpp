#include <iostream>
#include <LX/ArgParser.hpp>

LX::ArgParser::ArgParser(const std::vector<Param>& params)
    : m_Params(params)
{
}

void LX::ArgParser::Parse(const int argc, const char* const* argv)
{
    Filename = argv[0];

    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];
        bool flag_or_option = false;
        for (const auto& [name, desc, aliases, is_flag] : m_Params)
        {
            size_t j;
            for (j = 0; j < aliases.size(); ++j)
                if (arg == aliases[j])
                    break;
            if (j == aliases.size())
                continue;

            if (is_flag)
            {
                Flags[name] = true;
                flag_or_option = true;
                break;
            }

            Options[name] = argv[++i];
            flag_or_option = true;
            break;
        }

        if (flag_or_option)
            continue;

        Args.push_back(arg);
    }
}

void LX::ArgParser::Print()
{
    std::cout << "USAGE" << std::endl;
    std::cout << '\t' << Filename << " [OPTIONS] <arg>..." << std::endl;
    std::cout << "OPTIONS" << std::endl;
    fprintf(stdout, "\t%-20s %-20s %30s\n", "NAME", "OPTION", "DESCRIPTION");
    for (const auto& [name, desc, aliases, is_flag] : m_Params)
    {
        std::string a;
        for (const auto& alias : aliases)
            a += alias + ", ";
        a.pop_back();
        a.pop_back();
        fprintf(stdout, "\t%-20s %-20s %30s\n", name.c_str(), a.c_str(), desc.c_str());
    }
    fflush(stdout);
}
