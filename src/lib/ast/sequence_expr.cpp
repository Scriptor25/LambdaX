#include <LX/AST.hpp>
#include <LX/Builder.hpp>

LX::SequenceExpr::SequenceExpr(SourceLocation where, TypePtr type, std::vector<ExprPtr> children)
    : Expr(std::move(where), std::move(type)), Children(std::move(children))
{
}

std::ostream& LX::SequenceExpr::Print(std::ostream& os) const
{
    static unsigned depth = 0;
    static const auto spacing = []
    {
        std::string s;
        for (unsigned i = 0; i < depth; ++i)
            s += ' ';
        return s;
    };

    os << '(';
    if (Children.size() == 1)
        return Children[0]->Print(os) << ')';
    depth += 2;
    for (size_t i = 0; i < Children.size(); ++i)
    {
        if (i > 0) os << ',';
        Children[i]->Print(os << std::endl << spacing());
    }
    depth -= 2;
    return os << std::endl << spacing() << ')';
}

LX::ValuePtr LX::SequenceExpr::GenIR(Builder& builder) const
{
    builder.Push();
    ValuePtr value;
    for (const auto& child : Children)
        value = child->GenIR(builder);
    builder.Pop();
    return value;
}
