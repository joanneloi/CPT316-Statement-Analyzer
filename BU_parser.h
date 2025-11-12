#pragma once
#include "lexer.h"
#include <vector>
#include <string>
using namespace std;

// ---------------- Parse Tree Node ----------------
struct Node {
    string label;
    vector<Node*> children;
    string errorMsg; // store syntax error message if any

    Node(const string& lbl) : label(lbl) {}
};

class Parser {
public:
    explicit Parser(const vector<Token>& tokens);
    bool parse();

private:
    struct StackItem {
        string symbol;
        Node* node;
    };
    const vector<Token>& tokens;
    int pos;

    vector<StackItem> stack;

    void shift();
    bool tryReduce();

    void syntaxError(const string& message, int position);

};

void printTree(Node* root, int depth);

struct ParserError {
    string message;
    int position;
};

