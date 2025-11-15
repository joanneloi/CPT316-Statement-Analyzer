#pragma once
#include "lexer.h"
#include <vector>
#include <string>
using namespace std;

// ---------------- Parse Tree Node ----------------
struct Node {
    string label;
    vector<Node*> children;

    Node(const string& lbl) : label(lbl) {}
};

struct StackItem {
    string symbol;
    Node* node;
    int position;  // position of the token in the original input
};

struct ParseError {
    int errorPos;       // 0-based
    string message;     // error message
};


class Parser {
public:
    explicit Parser(const vector<Token>& tokens);
    void parse();

private:
    const vector<Token>& tokens;
    vector<ParseError> errors;
    int pos;
    int parenCount = 0;

    vector<StackItem> stack;

    void shift();
    bool tryReduce(vector<ParseError>& errors);
    void checkError();
    void printTree(Node* root, int depth);
    void printParseErrors(const vector<ParseError>& errors, const vector<StackItem>& stack);
};
