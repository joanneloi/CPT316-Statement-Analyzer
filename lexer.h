#pragma once
#include <string>
#include <vector>

using namespace std;

// ---------- Token model ----------
enum class TokenType {
    IDENTIFIER,         // single lowercase letter [a-z]
    NUMBER,             // integer literal
    PLUS, MINUS, MUL, DIV,
    ASSIGN,             // '='
    LPAREN, RPAREN,     // '(' ')'
    SEMICOLON,          // ';'
    INVALID,            // anything outside the language
    END                 // end of input
};

struct Token {
    TokenType type;
    string lexeme;  // exact text
    int position;        // 0-based index in the original input
};

struct TokenCounts {
    int identifiers = 0;
    int numbers = 0;
    int operators = 0;
    int assignment = 0;
    int parentheses = 0;
    int semicolons = 0;
    int invalids = 0;
        int total() const {
        return identifiers + numbers + operators + assignment + parentheses + semicolons + invalids;
    }
};

// ---------- LexicalError model ----------
struct LexicalError {
    int position;        // 0-based
    string message; // e.g., "invalid character '$'"
};

// ---------- Lexer ----------
class Lexer {
public:
    explicit Lexer(const string& source);

    // Produce tokens; any lexical errors are appended to 'errors'
    vector<Token> tokenize(vector<LexicalError>& errors);

private:
    const string s_;
    const int n_;
    int i_;

    static Token makeToken(TokenType t, const string& lex, int pos);
    static string printable(const string& x);
};

// Get string representation of TokenType
string tokenTypeName(TokenType t);
// Utility printing/counting functions
void printTokenStream(const std::vector<Token>& tokens);
TokenCounts countTokens(const std::vector<Token>& tokens);
void printCounts(const TokenCounts& c);

void printLexicalErrors(const std::vector<LexicalError>& errors);

