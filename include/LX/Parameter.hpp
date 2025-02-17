#pragma once

#include <LX/LX.hpp>

namespace LX
{
    struct Parameter
    {
        std::ostream& Print(std::ostream&) const;

        bool IsMutable;
        TypePtr Type;
        std::string Name;
    };
}
