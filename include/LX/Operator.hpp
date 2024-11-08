#pragma once

#include <LX/LX.hpp>

namespace LX
{
    ValuePtr OperatorLOr(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorLXOr(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorLAnd(Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypeLogical(Context&, const TypePtr&);

    ValuePtr OperatorEQ(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorNE(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorLT(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorLE(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorGT(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorGE(Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypeCmp(Context&, const TypePtr&);

    ValuePtr OperatorAdd(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorSub(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorMul(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorDiv(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorRem(Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypeArith(Context&, const TypePtr&);

    ValuePtr OperatorPow(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorRt(Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypePow(Context&, const TypePtr&);

    ValuePtr OperatorOr(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorXOr(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorAnd(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorShL(Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorShR(Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypeBitwise(Context&, const TypePtr&);

    ValuePtr OperatorNeg(Builder&, const ValuePtr&);
    TypePtr OperatorTypeNeg(Context&, const TypePtr&);
    ValuePtr OperatorNot(Builder&, const ValuePtr&);
    TypePtr OperatorTypeNot(Context&, const TypePtr&);
    ValuePtr OperatorLNot(Builder&, const ValuePtr&);
    TypePtr OperatorTypeLNot(Context&, const TypePtr&);

    ValuePtr OperatorRef(Builder&, const ValuePtr&);
    TypePtr OperatorTypeRef(Context&, const TypePtr&);
    ValuePtr OperatorDeref(Builder&, const ValuePtr&);
    TypePtr OperatorTypeDeref(Context&, const TypePtr&);
}
