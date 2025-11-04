#pragma once
#include "lexer.h"
#include <vector>
#include <string>
using namespace std;

// ---------------- Parse Tree Node ----------------
struct Node {
    string label;
    vector<Node*> children;
    Node(string l) : label(l) {}
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

};

void printTree(Node* root, int depth);