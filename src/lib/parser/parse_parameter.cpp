#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>

void LX::Parser::ParseParameter(Parameter& parameter)
{
    parameter.Type = ParseType();
    parameter.Name = Expect(TokenType_Symbol).StringValue;
}
