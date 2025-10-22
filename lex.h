#ifndef LEX_H
#define LEX_H

#include "tokennumbers.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class Lexer {
private:
    vector<Token> tokens;
    int index = 0;

public:
    Lexer(vector<Token> inputTokens) : tokens(inputTokens) {}

    Token getNextToken() {
        if (index < tokens.size())
            return tokens[index++];
        else
            return END;
    }

    Token peekToken() {
        if (index < tokens.size())
            return tokens[index];
        else
            return END;
    }
};

#endif
