#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::StmtPtr LX::Parser::ParseVariable(const std::string& name)
{
    Expect("=");
    return {};
}
