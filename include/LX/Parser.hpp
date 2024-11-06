#pragma once

#include <functional>
#include <string>
#include <vector>
#include <LX/Context.hpp>
#include <LX/LX.hpp>
#include <LX/SourceLocation.hpp>
#include <LX/Token.hpp>

namespace LX
{
    using Consumer = std::function<void(StmtPtr)>;

    class Parser
    {
    public:
        static void Parse(Context&, std::istream&, const std::string&, const Consumer&);

    private:
        Parser(Context&, std::istream&, const std::string&);

        int Get();
        int GetNewLine();
        void Escape();
        Token& Next();

        [[nodiscard]] bool AtEOF() const;
        [[nodiscard]] bool At(TokenType) const;
        [[nodiscard]] bool At(const std::string&) const;

        bool NextAt(TokenType);
        bool NextAt(const std::string&);

        Token Skip();
        Token Expect(TokenType);
        Token Expect(const std::string&);

        TypePtr& ParseType();
        bool ParseTypeList(std::vector<TypePtr>&, const std::string&);

        void ParseParameter(Parameter&);
        bool ParseParameterList(std::vector<Parameter>&, const std::string&);

        StmtPtr ParseStmt();
        StmtPtr ParseFunction(const std::string&);

        ExprPtr ParseExpr();
        void ParseExprList(std::vector<ExprPtr>&, const std::string&);

        ExprPtr ParseBinary(ExprPtr, unsigned);
        ExprPtr ParseOperand();
        ExprPtr ParsePrimary();

        Context& m_Ctx;
        std::istream& m_Stream;
        SourceLocation m_Where;
        Token m_Token;
        int m_Tok;
    };
}
