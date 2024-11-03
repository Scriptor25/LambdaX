#pragma once

#include <LX/LX.hpp>

namespace LX
{
    bool OperatorLOr(Builder&, const Value&, const Value&, Value&);
    bool OperatorLXOr(Builder&, const Value&, const Value&, Value&);
    bool OperatorLAnd(Builder&, const Value&, const Value&, Value&);
    bool OperatorEQ(Builder&, const Value&, const Value&, Value&);
    bool OperatorNE(Builder&, const Value&, const Value&, Value&);
    bool OperatorLT(Builder&, const Value&, const Value&, Value&);
    bool OperatorLE(Builder&, const Value&, const Value&, Value&);
    bool OperatorGT(Builder&, const Value&, const Value&, Value&);
    bool OperatorGE(Builder&, const Value&, const Value&, Value&);
    bool OperatorAdd(Builder&, const Value&, const Value&, Value&);
    bool OperatorSub(Builder&, const Value&, const Value&, Value&);
    bool OperatorMul(Builder&, const Value&, const Value&, Value&);
    bool OperatorDiv(Builder&, const Value&, const Value&, Value&);
    bool OperatorRem(Builder&, const Value&, const Value&, Value&);
    bool OperatorPow(Builder&, const Value&, const Value&, Value&);
    bool OperatorRt(Builder&, const Value&, const Value&, Value&);
    bool OperatorOr(Builder&, const Value&, const Value&, Value&);
    bool OperatorXOr(Builder&, const Value&, const Value&, Value&);
    bool OperatorAnd(Builder&, const Value&, const Value&, Value&);
    bool OperatorShL(Builder&, const Value&, const Value&, Value&);
    bool OperatorShR(Builder&, const Value&, const Value&, Value&);
}
