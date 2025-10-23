#pragma once
#include <string>
#include <vector>

using namespace std;

// ---------- Token model ----------
enum class TokenType {
    IDENTIFIER,      // single lowercase letter [a-z]
    NUMBER,          // integer literal
    PLUS, MINUS, MUL, DIV,
    ASSIGN,          // '='
    LPAREN, RPAREN,  // '(' ')'
    SEMICOLON,       // ';'
    INVALID          // anything outside the language
};

struct Token {
    TokenType type;
    string lexeme;  // exact text
    int position;        // 0-based index in the original input
};

// ---------- LexicalError model ----------
struct LexicalError {
    int position;        // 0-based
    string message; // e.g., "invalid character '$'"
};

// Friendly name for printing
string tokenTypeName(TokenType t);

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
