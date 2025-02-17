#include <LX/AST.hpp>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>
#include <LX/Type.hpp>

LX::ExprPtr LX::Parser::ParseBinary(ExprPtr left, const unsigned min_pre)
{
    static std::map<std::string, unsigned> PRE
    {
        {"=", 0},

        {"?", 1},
        {"??", 1},

        {"||", 2},

        {"^^", 3},

        {"&&", 4},

        {"|", 5},

        {"^", 6},

        {"&", 7},

        {"==", 8},
        {"!=", 8},

        {"<", 9},
        {"<=", 9},
        {">", 9},
        {">=", 9},

        {"<<", 10},
        {">>", 10},

        {"+", 11},
        {"-", 11},

        {"*", 12},
        {"/", 12},
        {"%", 12},

        {"**", 13},
        {"//", 13},
    };

    auto get_pre = [&]
    {
        return PRE[m_Token.StringValue];
    };

    while (At(TokenType_Operator) && get_pre() >= min_pre)
    {
        const auto [
            where_,
            type_,
            string_value_,
            integer_value_,
            float_value_
        ] = Skip();

        const auto operator_ = string_value_;
        const auto op_pre = PRE[operator_];

        auto right = ParseOperand();
        while (At(TokenType_Operator) && (get_pre() > op_pre || !get_pre()))
            right = ParseBinary(std::move(right), op_pre + (get_pre() ? 1 : 0));

        if (operator_ == "?")
        {
            Expect(TokenType_Colon);
            auto else_ = ParseExpr();
            left = std::make_unique<TernaryExpr>(
                where_,
                std::move(left),
                std::move(right),
                std::move(else_));
        }
        else if (operator_ == "??")
            left = std::make_unique<TernaryExpr>(
                where_,
                std::move(left),
                std::move(right),
                nullptr);
        else left = std::make_unique<BinaryExpr>(where_, operator_, std::move(left), std::move(right));
    }

    return left;
}
