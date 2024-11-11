#pragma once

#include <string>
#include <LX/LX.hpp>

namespace LX
{
    struct SourceLocation
    {
        void EmitDI(const Builder&) const;

        std::string Filename;
        size_t Row = 1;
        size_t Col = 1;
    };
}
