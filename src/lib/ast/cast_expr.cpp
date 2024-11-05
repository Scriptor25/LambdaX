#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

LX::CastExpr::CastExpr(ExprPtr src, TypePtr dst)
    : Src(std::move(src)), Dst(std::move(dst))
{
}

std::ostream& LX::CastExpr::Print(std::ostream& os) const
{
    return Dst->Print(Src->Print(os) << " as ");
}

void LX::CastExpr::GenIR(Builder& builder, Value& ref) const
{
    Value src;
    Src->GenIR(builder, src);
    builder.Cast(src, Dst, ref);
}
