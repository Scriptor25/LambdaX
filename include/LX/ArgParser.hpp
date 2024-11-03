#pragma once

#include <map>
#include <string>
#include <vector>

namespace LX
{
    struct Param
    {
        std::string Name;
        std::string Description;
        std::vector<std::string> Aliases;
        bool IsFlag = true;
    };

    class ArgParser
    {
    public:
        explicit ArgParser(const std::vector<Param>& params);

        void Parse(int argc, const char* const* argv);
        void Print();

        std::string Filename;
        std::vector<std::string> Args;
        std::map<std::string, bool> Flags;
        std::map<std::string, std::string> Options;

    private:
        std::vector<Param> m_Params;
    };
}
