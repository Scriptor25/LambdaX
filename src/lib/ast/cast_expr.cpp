#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

LX::CastExpr::CastExpr(TypePtr type, ExprPtr src)
    : Expr(std::move(type)), Src(std::move(src))
{
}

std::ostream& LX::CastExpr::Print(std::ostream& os) const
{
    return Type->Print(Src->Print(os) << " as ");
}

LX::ValuePtr LX::CastExpr::GenIR(Builder& builder) const
{
    const auto src = Src->GenIR(builder);
    return builder.Cast(src, Type);
}
