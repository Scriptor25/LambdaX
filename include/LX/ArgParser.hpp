#pragma once

#include <map>
#include <string>
#include <vector>

namespace LX
{
    struct Option
    {
        std::string Name;
        std::string Description;
        std::vector<std::string> Options;
        bool IsFlag = true;
    };

    class ArgParser
    {
    public:
        explicit ArgParser(const std::vector<Option>&);

        void Parse(int argc, const char* const* argv);
        void Print();

        std::string Filename;
        std::vector<std::string> Args;
        std::map<std::string, bool> Flags;
        std::map<std::string, std::string> Values;

    private:
        std::vector<Option> m_Options;
    };
}
