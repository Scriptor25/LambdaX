#include <LX/Parser.hpp>

LX::TypePtr LX::Parser::ParseType()
{
    const auto name = Expect(TokenType_Symbol).StringValue;
    return m_Ctx.GetType(name);
}
