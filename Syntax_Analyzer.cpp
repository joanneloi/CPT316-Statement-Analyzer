#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "tokennumbers.h"
#include "lex.h"

using namespace std;

Lexer *lexer;
Token currentToken;

void getNextToken() {
    currentToken = lexer->getNextToken();
}

void printNode(string name, int indent = 0) {
    cout << string(indent, ' ') << name << endl;
}

// Forward declarations
void expr(int indent = 0);
void exprPrime(int indent = 0);
void term(int indent = 0);
void termPrime(int indent = 0);
void factor(int indent = 0);

void stmt(int indent = 0) {
    printNode("<stmt>", indent);
    if (currentToken == ID) {
        printNode("id", indent + 2);
        getNextToken();
        if (currentToken == ASSIGN) {
            printNode("=", indent + 2);
            getNextToken();
            expr(indent + 2);
            if (currentToken == SEMI) {
                printNode(";", indent + 2);
                getNextToken();
            } else {
                cerr << "Error: missing ';'\n";
            }
        } else {
            cerr << "Error: missing '='\n";
        }
    } else {
        cerr << "Error: statement must start with identifier\n";
    }
}

void expr(int indent) {
    printNode("<expr>", indent);
    term(indent + 2);
    exprPrime(indent + 2);
}

void exprPrime(int indent) {
    if (currentToken == PLUS || currentToken == MINUS) {
        printNode("<expr'>", indent);
        if (currentToken == PLUS) printNode("+", indent + 2);
        else printNode("-", indent + 2);
        getNextToken();
        term(indent + 2);
        exprPrime(indent + 2);
    }
}

// <term> → <factor> <term’>
void term(int indent) {
    printNode("<term>", indent);
    factor(indent + 2);
    termPrime(indent + 2);
}

void termPrime(int indent) {
    if (currentToken == TIMES || currentToken == DIVIDE) {
        printNode("<term'>", indent);
        if (currentToken == TIMES) printNode("*", indent + 2);
        else printNode("/", indent + 2);
        getNextToken();
        factor(indent + 2);
        termPrime(indent + 2);
    }
}

void factor(int indent) {
    printNode("<factor>", indent);
    if (currentToken == ID) {
        printNode("id", indent + 2);
        getNextToken();
    } else if (currentToken == INT) {
        printNode("int", indent + 2);
        getNextToken();
    } else if (currentToken == LPAREN) {
        printNode("(", indent + 2);
        getNextToken();
        expr(indent + 2);
        if (currentToken == RPAREN) {
            printNode(")", indent + 2);
            getNextToken();
        } else {
            cerr << "Error: missing ')'\n";
        }
    } else {
        cerr << "Error: invalid factor\n";
    }
}

int main() {
    // Example: id = id + int * int ;
    vector<Token> tokens = {ID, ASSIGN, ID, PLUS, INT, TIMES, INT, SEMI, END};

    lexer = new Lexer(tokens);
    getNextToken();

    stmt();

    return 0;
}
