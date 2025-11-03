#include "lexer.h"
#include <cctype>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

using namespace std;

// ---------- tokenTypeName ----------
string tokenTypeName(TokenType t) {
    switch (t) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER:     return "NUMBER";
        case TokenType::PLUS:       return "OPERATOR(+)";
        case TokenType::MINUS:      return "OPERATOR(-)";
        case TokenType::MUL:        return "OPERATOR(*)";
        case TokenType::DIV:        return "OPERATOR(/)";
        case TokenType::ASSIGN:     return "ASSIGNMENT(=)";
        case TokenType::LPAREN:     return "LPAREN(()";
        case TokenType::RPAREN:     return "RPAREN())";
        case TokenType::SEMICOLON:  return "SEMICOLON(;)";
        case TokenType::END:        return "END";           //newly added need to check if the token is added into the string (for passing to parser) - added to lexer.h as well
        default:                    return "INVALID";
    }
}

// ---------- Token counting ----------
TokenCounts countTokens(const vector<Token>& tokens) {
    TokenCounts c;
    for (const auto& t : tokens) {
        switch (t.type) {
            case TokenType::IDENTIFIER: c.identifiers++; break;
            case TokenType::NUMBER:     c.numbers++;     break;
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::MUL:
            case TokenType::DIV:        c.operators++;   break;
            case TokenType::ASSIGN:     c.assignment++;  break;
            case TokenType::LPAREN:
            case TokenType::RPAREN:     c.parentheses++; break;
            case TokenType::SEMICOLON:  c.semicolons++;  break;
            case TokenType::INVALID:    c.invalids++;    break;
            case TokenType::END:                         break;
        }
    }
    return c;
}

// ---------- Lexer ----------
Lexer::Lexer(const string& source)
    : s_(source), n_(static_cast<int>(source.size())), i_(0) {}

Token Lexer::makeToken(TokenType t, const string& lex, int pos) {
    return Token{t, lex, pos};
}

string Lexer::printable(const string& x) {
    if (x == "\t") return "\\t";
    if (x == "\n") return "\\n";
    if (x == "\r") return "\\r";
    return x;
}

vector<Token> Lexer::tokenize(vector<LexicalError>& errors) {
    vector<Token> tokens;
    while (i_ < n_) {
        char c = s_[i_];

        // Skip whitespace
        if (isspace(static_cast<unsigned char>(c))) { ++i_; continue; }

        // Single-character tokens
        if (c == '+') { tokens.push_back(makeToken(TokenType::PLUS, "+", i_++)); continue; }
        if (c == '-') { tokens.push_back(makeToken(TokenType::MINUS, "-", i_++)); continue; }
        if (c == '*') { tokens.push_back(makeToken(TokenType::MUL,   "*", i_++)); continue; }
        if (c == '/') { tokens.push_back(makeToken(TokenType::DIV,   "/", i_++)); continue; }
        if (c == '=') { tokens.push_back(makeToken(TokenType::ASSIGN,"=", i_++)); continue; }
        if (c == '(') { tokens.push_back(makeToken(TokenType::LPAREN,"(", i_++)); continue; }
        if (c == ')') { tokens.push_back(makeToken(TokenType::RPAREN,")", i_++)); continue; }
        if (c == ';') { tokens.push_back(makeToken(TokenType::SEMICOLON,";", i_++)); continue; }

        // Identifier: exactly one lowercase letter [a-z]
        if (c >= 'a' && c <= 'z') {
            int start = i_;
            int j = i_;
            while (j < n_ && (s_[j] >= 'a' && s_[j] <= 'z')) j++; // read contiguous letters
            string word = s_.substr(start, j - start);
            if (static_cast<int>(word.size()) == 1) {
                tokens.push_back(makeToken(TokenType::IDENTIFIER, word, start));
            } else {
                errors.push_back({start, "invalid identifier '" + word + "' (identifiers must be single lowercase letter [a-z])"});
                tokens.push_back(makeToken(TokenType::INVALID, word, start));
            }
            i_ = j;

            // Disallow letter+digits (e.g., a123) → invalid tail
            if (i_ < n_ && isdigit(static_cast<unsigned char>(s_[i_]))) {
                int digStart = i_;
                int k = i_;
                while (k < n_ && isdigit(static_cast<unsigned char>(s_[k]))) k++;
                string tail = s_.substr(digStart, k - digStart);
                errors.push_back({digStart, "invalid alphanumeric sequence '" + word + tail + "' (identifiers cannot have digits)"});
                tokens.push_back(makeToken(TokenType::INVALID, tail, digStart));
                i_ = k;
            }
            continue;
        }

        // Number: one or more digits
        if (isdigit(static_cast<unsigned char>(c))) {
            int start = i_;
            int j = i_;
            while (j < n_ && isdigit(static_cast<unsigned char>(s_[j]))) j++;
            string num = s_.substr(start, j - start);
            tokens.push_back(makeToken(TokenType::NUMBER, num, start));
            i_ = j;

            // Disallow number followed immediately by letters (e.g., 123abc)
            if (i_ < n_ && (s_[i_] >= 'a' && s_[i_] <= 'z')) {
                int k = i_;
                while (k < n_ && (s_[k] >= 'a' && s_[k] <= 'z')) k++;
                string tail = s_.substr(i_, k - i_);
                errors.push_back({i_, "invalid identifier '" + num + tail + "' (numbers cannot be followed immediately by letters)"});
                tokens.push_back(makeToken(TokenType::INVALID, tail, i_));
                i_ = k;
            }
            continue;
        }

        // Anything else is invalid
        {
            int pos = i_;
            string bad(1, c);
            errors.push_back({pos, "invalid character '" + printable(bad) + "'"});
            tokens.push_back(makeToken(TokenType::INVALID, bad, pos));
            ++i_;
        }
    }
    tokens.push_back(makeToken(TokenType::END, "", i_)); // End of input token
    return tokens;
}

// ======== Token utilities and printing ========
void printTokenStream(const vector<Token>& tokens) {
    cout << left
         << setw(20) << "Token Type"
         << setw(20) << "Lexeme (Value)"
         << "Position\n";
    cout << string(55, '-') << "\n";

    for (const auto& t : tokens) {
        cout << left
             << setw(20) << tokenTypeName(t.type)
             << setw(20) << t.lexeme
             << t.position << "\n";
    }
    cout << "\n";
}

void printCounts(const TokenCounts& c) {
    cout << "Token Counts:\n----------------------\n";
    cout << "  Identifiers : " << c.identifiers  << "\n";
    cout << "  Numbers     : " << c.numbers      << "\n";
    cout << "  Operators   : " << c.operators    << "\n";
    cout << "  Assignment  : " << c.assignment   << "\n";
    cout << "  Parentheses : " << c.parentheses  << "\n";
    cout << "  Semicolons  : " << c.semicolons   << "\n";
    cout << "  Invalid     : " << c.invalids     << "\n";
    cout << "----------------------\n";
    cout << "  Total       : " << c.total()      << "\n\n";
}

void printLexicalErrors(const vector<LexicalError>& errors) {
    if (errors.empty()) return;
    cout << "Lexical Errors:\n";
    for (const auto& e : errors) {
        cout << "  LexicalError at position " << e.position << ": " << e.message << "\n";
    }
    cout << "\n";
}