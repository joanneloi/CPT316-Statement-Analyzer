#include "parser.h"
#include <iostream>

using std::cout;
using std::string;

// Constructor
Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens), pos_(0), indent_(0), valid_(true) {}

// Utility functions
const Token& Parser::peek() const {
    if (pos_ < (int)tokens_.size()) return tokens_[pos_];
    return tokens_.back(); // assume tokens has an END token
}

bool Parser::atEnd() const {
    return peek().type == TokenType::END;
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        pos_++;
        return true;
    }
    return false;
}

bool Parser::expect(TokenType type) {
    if (match(type)) return true;
    error("Expected token missing: " + tokenTypeName(type));
    valid_ = false;
    return false;
}

void Parser::error(const string& msg) {
    cout << "PARSER ERROR: " << msg << "\n";
}

// ---------- indentation helpers ----------
void Parser::printNode(const string& name) {
    cout << string(indent_, ' ') << name << "\n";
}
void Parser::indentPush() { indent_ += 2; }
void Parser::indentPop()  { indent_ = (indent_ >= 2 ? indent_ - 2 : 0); }

//--------------------------------------------
// Grammar implementation
//--------------------------------------------

// <stmt> → id = <expr> ;
bool Parser::stmt() {
    printNode("<stmt>");
    indentPush();

    if (!expect(TokenType::IDENTIFIER)) { indentPop(); return false; }
    printNode("id");

    if (!expect(TokenType::ASSIGN)) { indentPop(); return false; }
    printNode("=");

    if (!expr()) { indentPop(); return false; }

    if (!expect(TokenType::SEMICOLON)) { indentPop(); return false; }
    printNode(";");

    indentPop();
    return true;
}

// <expr> → <term> <expr'>
bool Parser::expr() {
    printNode("<expr>");
    indentPush();

    if (!term()) { indentPop(); return false; }
    if (!exprPrime()) { indentPop(); return false; }

    indentPop();
    return true;
}

// <expr'> → + <term> <expr'> | - <term> <expr'> | ε
bool Parser::exprPrime() {
    // print only if we're about to produce something OR to follow your desired format
    // You earlier wanted expr' printed even when empty; if you prefer that, uncomment the next two lines.
    // printNode("<expr'>");
    // indentPush();

    // We'll print <expr'> only when operator exists (this keeps output tidy but include expr' label).
    if (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
        printNode("<expr'>");
        indentPush();

        while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
            if (match(TokenType::PLUS)) {
                printNode("+");
            } else { // MINUS
                match(TokenType::MINUS);
                printNode("-");
            }

            if (!term()) { indentPop(); return false; }
        }

        // after processing chain of +/-
        if (!exprPrime()) { indentPop(); return false; } // recursive tail (though while loop already handles chain)
        indentPop();
        return true;
    }

    // epsilon: do nothing (no node printed)
    return true;
}

// <term> → <factor> <term'>
bool Parser::term() {
    printNode("<term>");
    indentPush();

    if (!factor()) { indentPop(); return false; }
    if (!termPrime()) { indentPop(); return false; }

    indentPop();
    return true;
}

// <term'> → * <factor> <term'> | / <factor> <term'> | ε
bool Parser::termPrime() {
    if (peek().type == TokenType::MUL || peek().type == TokenType::DIV) {
        printNode("<term'>");
        indentPush();

        while (peek().type == TokenType::MUL || peek().type == TokenType::DIV) {
            if (match(TokenType::MUL)) {
                printNode("*");
            } else {
                match(TokenType::DIV);
                printNode("/");
            }

            if (!factor()) { indentPop(); return false; }
        }

        // handle tail
        if (!termPrime()) { indentPop(); return false; } // tail recursion (safe)
        indentPop();
        return true;
    }

    // epsilon
    return true;
}

// <factor> → id | int | ( <expr> )
bool Parser::factor() {
    printNode("<factor>");
    indentPush();

    if (match(TokenType::IDENTIFIER)) {
        printNode("id");
        indentPop();
        return true;
    }
    if (match(TokenType::NUMBER)) {
        printNode("int");
        indentPop();
        return true;
    }
    if (match(TokenType::LPAREN)) {
        printNode("(");
        // parse inner expression at increased indent
        if (!expr()) { indentPop(); return false; }
        if (!expect(TokenType::RPAREN)) { indentPop(); return false; }
        printNode(")");
        indentPop();
        return true;
    }

    error("Invalid <factor> (expected id, int, or (expr)).");
    indentPop();
    return false;
}

// Entry point
bool Parser::parse() {
    bool result = stmt();
    if (!atEnd()) {
        error("Extra tokens after complete statement.");
        return false;
    }
    return result && valid_;
}
