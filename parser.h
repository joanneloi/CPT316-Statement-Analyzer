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
};

struct ParseResult {
    bool success;
    Node* parseTree;
    vector<StackItem> stack;  // can now use StackItem
    int errorPos;
    string message;
};


class Parser {
public:
    explicit Parser(const vector<Token>& tokens);
    ParseResult parse();

private:
    const vector<Token>& tokens;
    int pos;

    vector<StackItem> stack;

    void shift();
    bool tryReduce();

};

void printTree(Node* root, int depth);
void handleParseError(const vector<Token>& tokens);
void handleParseErrorFromStack(const ParseResult& result, const vector<Token>& tokens);
