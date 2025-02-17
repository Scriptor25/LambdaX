#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseOperand()
{
    auto ptr = ParsePrimary();
    while (true)
    {
        if (At(TokenType_ParenOpen))
        {
            ptr = ParseCall(std::move(ptr));
            continue;
        }

        if (At(TokenType_BracketOpen))
        {
            ptr = ParseSubscript(std::move(ptr));
            continue;
        }

        if (At("as"))
        {
            ptr = ParseCast(std::move(ptr));
            continue;
        }

        if (At(TokenType_Period) || At("!"))
        {
            ptr = ParseMember(std::move(ptr));
            continue;
        }

        return ptr;
    }
}
