#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include "trying.h"
using namespace std;

// Utility: print parse tree
void printTree(Node* node, int depth = 0) {
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << node->label << "\n";
    for (auto* child : node->children)
        printTree(child, depth + 1);
}

// ---------------- Bottom-Up Parser ----------------


Parser::Parser(const vector<Token>& t) : tokens(t), pos(0) {}

// --- parse loop: only one place for syntax checks ---
bool Parser::parse() {
    cout << "=== SHIFT-REDUCE PARSING START ===\n";

    while (true) {
        // acceptance
        if (tokens[pos].type == TokenType::END &&
            stack.size() == 1 &&
            stack.back().symbol == "<stmt>") {
            cout << "\nInput accepted.\n\nParse Tree:\n";
            printTree(stack.back().node);
            return true;
        }

        bool reduced = tryReduce();

        if (!reduced) {
            // If at end-of-input -> report appropriate message (once)
            if (tokens[pos].type == TokenType::END) {
                // if any stored node error -> print that (optional)
                for (auto &it : stack) if (!it.node->errorMsg.empty()) {
                    syntaxError(it.node->errorMsg, pos); // overload below expects pos
                    return false;
                }
                // Check if the first token (bottom of stack) is not an identifier (invalid start)
                if (!stack.empty() && stack.front().symbol != "id") {
                    cout << "SyntaxError at beginning of input: missing an identifier ('id').\n";
                }
                // Check if last shifted symbol was a semicolon
                if (!stack.empty() && stack.back().symbol != ";") {
                    cout << "SyntaxError at end of input: missing ';'\n";
                }
                
                return false;
            }
                        
            // --- NEW: centralized lookahead checks before shifting ---
            Token look = tokens[pos];
            string current = look.lexeme; // token text like "+", "*", "id" value, etc.
            string top = stack.empty() ? "" : stack.back().symbol;
            string next = (pos + 1 < (int)tokens.size()) ? tokens[pos + 1].lexeme : "END";

            // If next is a binary operator but top is not a valid left operand => error
            if (current == "+" || current == "-" || current == "*" || current == "/") {
                bool topIsOperand = (top == "<expr>" || top == "<term>" ||
                                     top == "<factor>" || top == "id" ||
                                     top == "int" || top == ")");
                bool nextIsOperand = (next == "id" || next == "int" || next == "(");   
                if (!topIsOperand) {
                    syntaxError("expected operand before '" + current + "'", pos + 1);
                    return false;
                }
                if (!nextIsOperand) {
                    syntaxError("expected operand after '" + current + "'", pos + 1);
                    return false;
                }
            }

            // Passed checks -> safe to shift
            shift();
        }
    }
}

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

bool Parser::tryReduce() {
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

            cout << "Reduce: " << lhs << " → ";
            for (auto s : rhs) cout << s << " ";
            cout << "\n";

            return true;
        }
    }
    return false;
}

// print a single, clear error (pos points at the token that caused the problem)
void Parser::syntaxError(const string &message, int errorPos) {
    // If you want the reported token to be the last shifted token, use errorPos-1 when errorPos>0.
    int reportPos = (errorPos > 0) ? errorPos - 1 : errorPos;

    if (reportPos < (int)tokens.size()) {
        cout << "SyntaxError at position " << reportPos
             << ": " << message << "\n";
            //  << " near '" << tokens[reportPos].lexeme << "'\n";
    } else {
        cout << "SyntaxError at end of input: " << message << "\n";
    }
}
