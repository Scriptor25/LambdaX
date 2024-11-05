#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::ExprPtr LX::Parser::ParseBinary(ExprPtr lhs, const unsigned min_pre)
{
    static std::map<std::string, unsigned> pre
    {
        {"||", 0},

        {"^^", 1},

        {"&&", 2},

        {"|", 3},

        {"^", 4},

        {"&", 5},

        {"==", 6},
        {"!=", 6},

        {"<", 7},
        {"<=", 7},
        {">", 7},
        {">=", 7},

        {"<<", 8},
        {">>", 8},

        {"+", 9},
        {"-", 9},

        {"*", 10},
        {"/", 10},
        {"%", 10},

        {"**", 11},
        {"//", 11},
    };

    auto get_pre = [&]
    {
        return pre[m_Token.StringValue];
    };

    while (At(TokenType_Operator) && get_pre() >= min_pre)
    {
        const auto op = Skip().StringValue;
        const auto op_pre = pre[op];

        auto rhs = ParseOperand();
        while (At(TokenType_Operator) && get_pre() > op_pre)
            rhs = ParseBinary(std::move(rhs), op_pre + 1);

        lhs = std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
    }

    return lhs;
}
