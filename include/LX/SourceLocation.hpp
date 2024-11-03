#pragma once

#include <string>

namespace LX
{
    struct SourceLocation
    {
        std::string Filename;
        size_t Row = 1;
        size_t Col = 1;
    };
}
