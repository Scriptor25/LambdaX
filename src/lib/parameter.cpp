#include <LX/Parameter.hpp>
#include <LX/Type.hpp>

std::ostream& LX::Parameter::Print(std::ostream& os) const
{
    if (Name.empty()) return Type->Print(os);
    return Type->Print(os) << ' ' << Name;
}
