#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Type.hpp>

LX::CastExpr::CastExpr(SourceLocation where, ExprPtr source, TypePtr dest)
    : Expr(std::move(where)), Source(std::move(source)), Dest(std::move(dest))
{
}

std::ostream& LX::CastExpr::Print(std::ostream& os) const
{
    return Dest->Print(Source->Print(os) << " as ");
}

LX::ValuePtr LX::CastExpr::GenIR(Builder& builder) const
{
    const auto src = Source->GenIR(builder);
    Where.EmitDI(builder);
    return builder.CreateCast(Where, src, Dest);
}
