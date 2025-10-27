#pragma once
#include "lexer.h"
#include <vector>
#include <string>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    bool parse(); // entry point (for <stmt>)

private:
    const std::vector<Token>& tokens_;
    int pos_; // current token index

    // grammar rule methods
    bool stmt();
    bool expr();
    bool exprPrime();
    bool term();
    bool termPrime();
    bool factor();

    // utility
    bool match(TokenType type);
    bool expect(TokenType type);
    const Token& peek() const;
    bool atEnd() const;
    void error(const std::string& msg);

    bool valid_;
};
