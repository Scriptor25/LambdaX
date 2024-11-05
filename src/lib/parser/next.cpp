#include <istream>
#include <map>
#include <LX/Error.hpp>
#include <LX/Parser.hpp>

static bool is_compound_operator(const int c)
{
    static std::map<int, bool> compound_operators
    {
        {'=', true},
        {'<', true},
        {'>', true},
        {'+', true},
        {'-', true},
        {'*', true},
        {'/', true},
        {'&', true},
        {'|', true},
        {'^', true},
    };
    return compound_operators[c];
}

int LX::Parser::Get()
{
    ++m_Where.Col;
    return m_Stream.get();
}

int LX::Parser::GetNewLine()
{
    m_Where.Col = 1;
    ++m_Where.Row;
    return m_Stream.get();
}

LX::Token& LX::Parser::Next()
{
    enum State
    {
        State_Bin,
        State_Comment,
        State_Dec,
        State_Hex,
        State_Idle,
        State_Oct,
        State_Operator,
        State_String,
        State_Symbol,
    };

    State state = State_Idle;
    SourceLocation where;
    std::string value;
    bool is_float = false;

    while (m_Tok >= 0 || state != State_Idle)
    {
        switch (state)
        {
        case State_Idle:
            switch (m_Tok)
            {
            case '\n':
                m_Tok = GetNewLine();
                continue;

            case '#':
                state = State_Comment;
                break;

            case '=':
            case '<':
            case '>':
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '&':
            case '|':
            case '^':
            case '!':
            case '~':
                state = State_Operator;
                where = m_Where;
                value += static_cast<char>(m_Tok);
                break;

            case '(':
            case ')':
            case '{':
            case '}':
            case '[':
            case ']':
            case '?':
            case '.':
            case ':':
            case ',':
            case ';':
            case '$':
                where = m_Where;
                value += static_cast<char>(m_Tok);
                m_Tok = Get();
                return m_Token = {where, TokenType_Other, value};

            case '0':
                where = m_Where;
                m_Tok = Get();
                if (m_Tok == '.')
                {
                    state = State_Dec;
                    is_float = true;
                    value = "0.";
                    break;
                }
                if (m_Tok == 'b' || m_Tok == 'B')
                {
                    state = State_Bin;
                    break;
                }
                if (m_Tok == 'x' || m_Tok == 'X')
                {
                    state = State_Hex;
                    break;
                }
                state = State_Oct;
                value = "0";
                continue;

            case '"':
                where = m_Where;
                state = State_String;
                break;

            default:
                where = m_Where;
                if (isdigit(m_Tok))
                {
                    state = State_Dec;
                    value += static_cast<char>(m_Tok);
                    break;
                }
                if (isalpha(m_Tok) || m_Tok == '_')
                {
                    state = State_Symbol;
                    value += static_cast<char>(m_Tok);
                    break;
                }
                break;
            }
            break;

        case State_Comment:
            if (m_Tok == '\n')
            {
                state = State_Idle;
                m_Tok = GetNewLine();
                continue;
            }
            break;

        case State_Symbol:
            if (!isalnum(m_Tok) && m_Tok != '_')
                return m_Token = {where, TokenType_Symbol, value};
            value += static_cast<char>(m_Tok);
            break;

        case State_Bin:
            if (m_Tok != '0' && m_Tok != '1')
                return m_Token = {where, TokenType_Int, value, std::stoull(value, nullptr, 2)};
            value += static_cast<char>(m_Tok);
            break;

        case State_Oct:
            if (m_Tok < '0' || '7' < m_Tok)
                return m_Token = {where, TokenType_Int, value, std::stoull(value, nullptr, 8)};
            value += static_cast<char>(m_Tok);
            break;

        case State_Dec:
            if (!isdigit(m_Tok) && m_Tok != '.')
            {
                if (is_float)
                    return m_Token = {where, TokenType_Float, value, 0, std::stod(value)};
                return m_Token = {where, TokenType_Int, value, std::stoull(value)};
            }
            if (m_Tok == '.')
            {
                if (is_float)
                    Error("only one floating point per token is permitted");
                is_float = true;
            }
            value += static_cast<char>(m_Tok);
            break;

        case State_Hex:
            if (!isxdigit(m_Tok))
                return m_Token = {where, TokenType_Int, value, std::stoull(value, nullptr, 16)};
            value += static_cast<char>(m_Tok);
            break;

        case State_Operator:
            if (!is_compound_operator(m_Tok))
                return m_Token = {where, TokenType_Operator, value};
            value += static_cast<char>(m_Tok);
            break;

        case State_String:
            if (m_Tok == '"')
            {
                m_Tok = Get();
                return m_Token = {where, TokenType_String, value};
            }
            value += static_cast<char>(m_Tok);
            break;
        }

        m_Tok = Get();
    }

    return m_Token = {m_Where};
}
