#include <LX/AST.hpp>

LX::SequenceExpr::SequenceExpr(std::vector<ExprPtr> children)
    : Children(std::move(children))
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

    os << spacing() << '(';
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

void LX::SequenceExpr::GenIR(Builder& builder, Value& ref) const
{
    for (const auto& child : Children)
        child->GenIR(builder, ref);
}
