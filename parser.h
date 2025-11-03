#pragma once
#include "lexer.h"
#include <vector>
#include <string>

struct Node {
    string label;
    vector<Node*> children;
    Node(string l) : label(l) {}
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    bool parse(); // entry point (for <stmt>)
    
    void printTree(Node* root, int indent = 0);

private:
    const std::vector<Token>& tokens_;
    int pos_; // current token index

    // grammar rule methods (same names)
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

    // indentation state for pretty printing
    int indent_;

    void printNode(const std::string& name); // prints at current indent
    void indentPush(); // increase indent
    void indentPop();  // decrease indent

    bool valid_;

    //To eliminate printing of parse tree with PRIME functions
    Node* parseE();
    Node* parseT();
    Node* parseF();
    
    Token current() const;
    void advance();
    
};