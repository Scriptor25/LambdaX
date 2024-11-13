#include <LX/Parameter.hpp>
#include <LX/Type.hpp>

std::ostream& LX::Parameter::Print(std::ostream& os) const
{
    if (IsMutable) os << "mut ";
    Type->Print(os);
    if (!Name.empty()) os << ' ' << Name;
    return os;
}
