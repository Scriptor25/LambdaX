#include <LX/Parameter.hpp>
#include <LX/Type.hpp>

std::ostream& LX::Parameter::Print(std::ostream& os) const
{
    return Type->Print(os) << ' ' << Name;
}
