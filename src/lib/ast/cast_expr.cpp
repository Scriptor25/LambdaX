#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

LX::CastExpr::CastExpr(SourceLocation where, TypePtr type, ExprPtr source)
    : Expr(std::move(where), std::move(type)), Source(std::move(source))
{
}

std::ostream& LX::CastExpr::Print(std::ostream& os) const
{
    return Type->Print(Source->Print(os) << " as ");
}

LX::ValuePtr LX::CastExpr::GenIR(Builder& builder) const
{
    const auto src = Source->GenIR(builder);
    return builder.Cast(Where, src, Type);
}
