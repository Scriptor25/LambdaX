#pragma once

#include <functional>
#include <string>
#include <vector>
#include <LX/LX.hpp>
#include <LX/SourceLocation.hpp>
#include <LX/Token.hpp>

namespace LX
{
    using Consumer = std::function<void(StmtPtr&&)>;

    class Parser
    {
    public:
        static void Parse(Context&, std::istream&, const std::string&, const Consumer&, bool is_imported = false);

    private:
        Parser(Context&, std::istream&, const std::string&, bool is_imported);

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

        TypePtr ParseType();

        void ParseParameter(Parameter&);
        bool ParseParameterList(std::vector<Parameter>&, TokenType);

        void ParseFunction(Function&);

        StmtPtr Parse();
        StmtPtr ParseImport();
        StmtPtr ParseGlobal();

        ExprPtr ParseExpr();
        void ParseExprList(std::vector<ExprPtr>&, const std::string&);

        ExprPtr ParseBinary(ExprPtr, unsigned);
        ExprPtr ParseOperand();

        ExprPtr ParseCall(ExprPtr);
        ExprPtr ParseSubscript(ExprPtr);
        ExprPtr ParseCast(ExprPtr);
        ExprPtr ParseMember(ExprPtr);

        ExprPtr ParsePrimary();

        ExprPtr ParseMut();
        ExprPtr ParseSwitch();
        ExprPtr ParseSizeOf();
        ExprPtr ParseNameOf();
        ExprPtr ParseNew();
        ExprPtr ParseDel();
        ExprPtr ParseSequence();
        ExprPtr ParseConstStruct();
        ExprPtr ParseConstFunction();
        ExprPtr ParseUnary();
        ExprPtr ParseSymbol();

        bool m_IsImported;
        Context& m_Ctx;
        std::istream& m_Stream;
        SourceLocation m_Where;
        Token m_Token;
        int m_Tok;
    };
}
