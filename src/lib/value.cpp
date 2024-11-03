#include <LX/Value.hpp>

bool LX::Value::operator!() const
{
    return !Type || !TypeIR || !ValueIR;
}
