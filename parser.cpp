#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include "parser.h"
using namespace std;

Parser::Parser(const vector<Token>& t) : tokens(t), pos(0) {}

// --- parse loop: only one place for syntax checks ---
void Parser::parse() {
    cout << "=== SHIFT-REDUCE PARSING START ===\n";

    while (true) {
        // acceptance
        if (tokens[pos].type == TokenType::END &&
            stack.size() == 1 &&
            stack.back().symbol == "<stmt>") {
            cout << "\nInput accepted.\n\nParse Tree:\n";
            printTree(stack.back().node, 0);
            return;
        }

        bool reduced = tryReduce(errors);

        if (!reduced) {
            // If at end-of-input -> report appropriate message (once)
            if (tokens[pos].type == TokenType::END) {
                printParseErrors(errors, stack);
                return;
            }
            // Passed checks -> safe to shift
            shift();
        }
    }
}

// Shift next token onto stack
void Parser::shift() {
    Token t = tokens[pos++];
    string symbol;

    switch (t.type) {
        case TokenType::IDENTIFIER: symbol = "id"; break;
        case TokenType::NUMBER:     symbol = "int"; break;
        case TokenType::PLUS:       symbol = "+"; break;
        case TokenType::MINUS:      symbol = "-"; break;
        case TokenType::MUL:        symbol = "*"; break;
        case TokenType::DIV:        symbol = "/"; break;
        case TokenType::ASSIGN:     symbol = "="; break;
        case TokenType::LPAREN:     symbol = "("; break;
        case TokenType::RPAREN:     symbol = ")"; break;
        case TokenType::SEMICOLON:  symbol = ";"; break;
        default: symbol = "invalid"; break;
    }

    Node* node = new Node(symbol);
    stack.push_back({symbol, node});
    cout << "Shift: " << symbol << "\n";
}

bool Parser::tryReduce(vector<ParseError>& errors) {
    // Grammar rules (RHS → LHS)
    vector<pair<vector<string>, string>> rules = {
        {{"id", "=", "<expr>", ";"}, "<stmt>"},
        {{"<expr>", "+", "<term>"}, "<expr>"},
        {{"<expr>", "-", "<term>"}, "<expr>"},
        {{"<term>", "*", "<factor>"}, "<term>"},
        {{"<term>", "/", "<factor>"}, "<term>"},
        {{"<term>"}, "<expr>"},
        {{"<factor>"}, "<term>"},
        {{"(", "<expr>", ")"}, "<factor>"},
        {{"int"}, "<factor>"},
        {{"id"}, "<factor>"}
    };

    for (auto& rule : rules) {
        auto rhs = rule.first;
        string lhs = rule.second;

        if (stack.size() < rhs.size()) continue;

        bool match = true;
        for (int i = 0; i < rhs.size(); i++) {
            if (stack[stack.size() - rhs.size() + i].symbol != rhs[i]) {
                match = false;
                break;
            }
        }

        if (match) {
            // Prevent premature reduction of 'id' into <factor> when next token is '='
            if (pos < tokens.size()) {
                string next = tokens[pos].lexeme;
                // Prevent reducing <term> → <factor> if next is * or /
                if (rhs == vector<string>{"<term>"} && (next == "*" || next == "/"))
                    continue;
                // Prevent reducing id → <factor> when followed by '=' 
                if (rhs == vector<string>{"id"} && next == "=")
                    continue;
            }

            // --- Perform Reduction ---
            Node* parent = new Node(lhs);
            for (int i = 0; i < rhs.size(); i++) {
                parent->children.push_back(stack[stack.size() - rhs.size() + i].node);
            }
            for (int i = 0; i < rhs.size(); i++) stack.pop_back();
            stack.push_back({lhs, parent});

            return true;
        } 
       
    }
    checkError();
    return false;
}

void Parser::checkError() {
    // --- OPERATOR OPERAND CHECK ---
    if (stack.empty()) return;
    string left = stack[stack.size()-2].symbol;
    int currentPos = pos-1;
    if(stack.back().node->label == "+" || stack.back().node->label == "-" ||
       stack.back().node->label == "*" || stack.back().node->label == "/"){
        if (left != "<expr>" && left != "<term>"){
            errors.push_back({pos - 1, "missing operand before " + tokens[currentPos].lexeme});
        }    
        
        if (tokens[pos].type != TokenType::NUMBER && tokens[pos].type != TokenType::IDENTIFIER){
            errors.push_back({pos -1 , "missing operand after " + tokens[currentPos].lexeme});
        }
    }    

    // --- PARENTHESIS ERROR CHECK: Missing ')' ---
    if (pos < tokens.size() && tokens[pos].lexeme == "(") {
        // Look backward in stack for "("
        bool hasCloseParen = false;
        for (int i = stack.size() - 1; i >= 0; --i) {
            if (stack[i].symbol == ")") {
                hasCloseParen = true;
                break;
            }
        }

        if (!hasCloseParen){
            int found = 0;
            int errorPos = pos;  // default fallback

            for (int i = pos; i < tokens.size(); i++) {
                string t = tokens[i].lexeme;

                bool isBreak =
                    (t == "+" || t == "-" || t == "*" ||
                    t == "/" || t == ";" || t == ")");

                if (isBreak) {
                    found++;
                    if (found == 2) {       // second occurrence
                        errorPos = i;
                        break;
                    }
                }
            }
            string errorToken = tokens[errorPos].lexeme;

            errors.push_back({errorPos, "expected ')' before '" + errorToken + "'" });
            return;
        }

    }

    // --- PARENTHESIS ERROR CHECK: Missing '(' ---
    if (pos < tokens.size() && tokens[pos].lexeme == ")") {
        // Look backward in stack for "("
        bool hasOpenParen = false;
        for (int i = stack.size() - 1; i >= 0; --i) {
            if (stack[i].symbol == "(") {
                hasOpenParen = true;
                break;
            }
        }

        // If there is no '(' in the stack, then we cannot match ')'
        if (!hasOpenParen) {
            int errorPos = tokens[pos].position; // position of ')'
            errors.push_back({
                errorPos,
                "expected '(' before ')'"
            });
            return;
        }
    }
}

//print parse tree
void Parser::printTree(Node* node, int depth) {
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << node->label << "\n";
    for (auto* child : node->children)
        printTree(child, depth + 1);
}

//print parse errors
void Parser::printParseErrors(const vector<ParseError>& errors, const vector<StackItem>& stack){
    // Check if the first token (bottom of stack) is not an identifier (invalid start)
    cout << "\nInput rejected.\n";
    cout << "Parse Errors:\n";
    if (!stack.empty() && stack.front().symbol != "id") {
        cout << "SyntaxError at beginning of input: missing an identifier ('id').\n";
    }
    // Check if last shifted symbol was a semicolon
    if (!stack.empty() && stack.back().symbol != ";") {
        cout << "SyntaxError at end of input: missing ';'\n";
    }
    
    if (errors.empty()) return;
    for (const auto& e : errors) {
        cout << "   ParseError at position " << e.errorPos << ": " << e.message << "\n";
    }
    cout << "\n";
}