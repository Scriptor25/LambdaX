#include <iostream>
#include <LX/ArgParser.hpp>

LX::ArgParser::ArgParser(const std::vector<Option>& options)
    : m_Options(options)
{
}

void LX::ArgParser::Parse(const int argc, const char* const* argv)
{
    Filename = argv[0];

    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];
        bool flag_or_value = false;
        for (const auto& [name, desc, options, is_flag] : m_Options)
        {
            size_t j;
            for (j = 0; j < options.size(); ++j)
                if (arg == options[j])
                    break;
            if (j == options.size())
                continue;

            if (is_flag)
            {
                Flags[name] = true;
                flag_or_value = true;
                break;
            }

            Values[name] = argv[++i];
            flag_or_value = true;
            break;
        }

        if (flag_or_value)
            continue;

        Args.push_back(arg);
    }
}

void LX::ArgParser::Print()
{
    fprintf(stderr, "USAGE\n");
    fprintf(stderr, "\t%s [OPTIONS] <arg>...\n", Filename.c_str());
    fprintf(stderr, "OPTIONS\n");

    int option_len = 0;
    int desc_len = 0;
    for (const auto& [name, desc, options, is_flag] : m_Options)
    {
        int option = 0;
        for (unsigned i = 0; i < options.size(); ++i)
        {
            if (i > 0) option += 2;
            option += static_cast<int>(options[i].length());
        }
        option_len = std::max(option_len, option);
        desc_len = std::max(desc_len, static_cast<int>(desc.length()));
    }

    fprintf(stderr, "\t%-*s  %*s\n", option_len, "OPTION", desc_len, "DESCRIPTION");
    for (const auto& [name, desc, options, is_flag] : m_Options)
    {
        std::string option;
        for (size_t i = 0; i < options.size(); ++i)
        {
            if (i > 0) option += ", ";
            option += options[i];
        }
        fprintf(stderr, "\t%-*s  %*s\n", option_len, option.c_str(), desc_len, desc.c_str());
    }
    fflush(stderr);
}
