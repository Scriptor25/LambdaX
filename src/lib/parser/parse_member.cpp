#include <LX/AST.hpp>
#include <LX/Parser.hpp>

LX::ExprPtr LX::Parser::ParseMember(ExprPtr ptr)
{
    const auto where = m_Token.Where;

    const auto deref = NextAt("!");
    if (!deref) Expect(".");

    const auto member = Expect(TokenType_Symbol).StringValue;

    return std::make_unique<MemberExpr>(where, std::move(ptr), member, deref);
}
