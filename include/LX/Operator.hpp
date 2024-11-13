#pragma once

#include <LX/LX.hpp>

namespace LX
{
    ValuePtr OperatorLOr(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorLXOr(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorLAnd(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypeLogical(const SourceLocation&, Context&, const TypePtr&);

    ValuePtr OperatorEQ(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorNE(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorLT(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorLE(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorGT(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorGE(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypeCmp(const SourceLocation&, Context&, const TypePtr&);

    ValuePtr OperatorAdd(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorSub(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorMul(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorDiv(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorRem(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypeArith(const SourceLocation&, Context&, const TypePtr&);

    ValuePtr OperatorPow(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorRt(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypePow(const SourceLocation&, Context&, const TypePtr&);

    ValuePtr OperatorOr(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorXOr(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorAnd(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorShL(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    ValuePtr OperatorShR(const SourceLocation&, Builder&, const ValuePtr&, const ValuePtr&);
    TypePtr OperatorTypeBitwise(const SourceLocation&, Context&, const TypePtr&);

    ValuePtr OperatorNeg(const SourceLocation&, Builder&, const ValuePtr&);
    TypePtr OperatorTypeNeg(const SourceLocation&, Context&, const TypePtr&);
    ValuePtr OperatorNot(const SourceLocation&, Builder&, const ValuePtr&);
    TypePtr OperatorTypeNot(const SourceLocation&, Context&, const TypePtr&);
    ValuePtr OperatorLNot(const SourceLocation&, Builder&, const ValuePtr&);
    TypePtr OperatorTypeLNot(const SourceLocation&, Context&, const TypePtr&);

    ValuePtr OperatorRef(const SourceLocation&, Builder&, const ValuePtr&);
    TypePtr OperatorTypeRef(const SourceLocation&, Context&, const TypePtr&);
    ValuePtr OperatorDeref(const SourceLocation&, Builder&, const ValuePtr&);
    TypePtr OperatorTypeDeref(const SourceLocation&, Context&, const TypePtr&);
}
