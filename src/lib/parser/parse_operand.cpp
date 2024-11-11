#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseOperand()
{
    auto ptr = ParsePrimary();
    while (true)
    {
        if (At("("))
        {
            ptr = ParseCall(std::move(ptr));
            continue;
        }

        if (At("["))
        {
            ptr = ParseSubscript(std::move(ptr));
            continue;
        }

        if (At("as"))
        {
            ptr = ParseCast(std::move(ptr));
            continue;
        }

        if (At(".") || At("!"))
        {
            ptr = ParseMember(std::move(ptr));
            continue;
        }

        return ptr;
    }
}
