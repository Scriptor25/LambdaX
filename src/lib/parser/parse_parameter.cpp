#include <LX/Parameter.hpp>
#include <LX/Parser.hpp>

void LX::Parser::ParseParameter(Parameter& parameter)
{
    parameter.Type = ParseType();
    if (At(TokenType_Symbol))
        parameter.Name = Skip().StringValue;
}
