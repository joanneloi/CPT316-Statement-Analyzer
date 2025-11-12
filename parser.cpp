#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include "parser.h"
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
ParseResult Parser::parse() {
    ParseResult result;
    result.success = false;
    result.parseTree = nullptr;

    while (true) {
        // Accepted input
        if (tokens[pos].type == TokenType::END &&
            stack.size() == 1 &&
            stack.back().symbol == "<stmt>") {
            result.success = true;
            result.parseTree = stack.back().node;
            return result;
        }

        bool reduced = tryReduce();

        if (!reduced) {
            // Safe shift
            if (tokens[pos].type == TokenType::END) {
                // Stop parsing, input ended
                result.success = false;
                result.stack = stack;  // capture current stack
                result.errorPos = pos; // last token processed
                return result;
            }
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
void handleParseError(const vector<Token>& tokens) {
    cout << "Parsing failed. Analyzing tokens for possible syntax errors...\n";

    // Example: simple heuristic checks
    if (tokens.empty()) {
        cout << "No tokens found.\n";
        return;
    }

    // Check for missing identifier at start
    if (tokens[0].type != TokenType::IDENTIFIER) {
        cout << "SyntaxError: expected identifier at beginning, found '"
             << tokens[0].lexeme << "'\n";
        return;
    }

    // Check for missing semicolon at end
    if (tokens[tokens.size()-2].type != TokenType::SEMICOLON) { 
        // last token is END
        cout << "SyntaxError: expected ';' at end of statement\n";
        return;
    }

    // General operator-operand mismatch
    for (size_t i = 0; i < tokens.size()-1; i++) {
        bool isOperator = (tokens[i].type == TokenType::PLUS || tokens[i].type == TokenType::MINUS ||
                           tokens[i].type == TokenType::MUL || tokens[i].type == TokenType::DIV);
        if (isOperator) {
            // check left operand exists
            if (i == 0 || !(tokens[i-1].type == TokenType::IDENTIFIER || tokens[i-1].type == TokenType::NUMBER || tokens[i-1].type == TokenType::RPAREN)) {
                cout << "SyntaxError: expected operand before '" << tokens[i].lexeme
                     << "' at position " << i << "\n";
                return;
            }
            // check right operand exists
            if (!(tokens[i+1].type == TokenType::IDENTIFIER || tokens[i+1].type == TokenType::NUMBER || tokens[i+1].type == TokenType::LPAREN)) {
                cout << "SyntaxError: expected operand after '" << tokens[i].lexeme
                     << "' at position " << i << "\n";
                return;
            }
        }
    }

    // Parentheses tracking
    int parenCount = 0;

    // Check for operator-operand mismatch and parentheses
    for (size_t i = 0; i < tokens.size()-1; i++) {
        Token curr = tokens[i];
        Token next = tokens[i+1];

        // Track parentheses
        if (curr.type == TokenType::LPAREN) parenCount++;
        if (curr.type == TokenType::RPAREN) parenCount--;

        if (parenCount < 0) {
            cout << "SyntaxError: unmatched ')' at position " << i << "\n";
            return;
        }

        // Operator checks
        if ((curr.type == TokenType::PLUS || curr.type == TokenType::MINUS ||
             curr.type == TokenType::MUL || curr.type == TokenType::DIV) &&
            (next.type != TokenType::NUMBER && next.type != TokenType::IDENTIFIER &&
             next.type != TokenType::LPAREN)) 
        {
            cout << "SyntaxError: expected operand after '" << curr.lexeme
                 << "' at position " << i << "\n";
            return;
        }
    }

    if (parenCount > 0) {
        cout << "SyntaxError: unmatched '(' in input\n";
        return;
    }

    cout << "Unable to determine exact syntax error.\n";
}

// void handleParseErrorFromStack(const ParseResult& res, const vector<Token>& tokens) {
//     cout << "Parsing failed. Analyzing last reduced state...\n";

//     if (res.stack.empty()) {
//         cout << "No reductions performed.\n";
//         return;
//     }

//     // Check for operators without operands
//     for (size_t i = 0; i < res.stack.size(); i++) {
//         const string& sym = res.stack[i].symbol;
//         if (sym == "+" || sym == "-" || sym == "*" || sym == "/") {
//             if (i == 0 || i == res.stack.size() - 1) {
//                 cout << "SyntaxError: operator '" << sym 
//                      << "' missing operand at position " << i << "\n";
//                 return;
//             }
//             const string& left = res.stack[i-1].symbol;
//             const string& right = res.stack[i+1].symbol;
//             bool leftIsOperand = (left == "<expr>" || left == "<term>" ||
//                                   left == "<factor>" || left == "id" || left == "int" || left == ")");
//             bool rightIsOperand = (right == "<expr>" || right == "<term>" ||
//                                    right == "<factor>" || right == "id" || right == "int" || right == "(");
//             if (!leftIsOperand) {
//                 cout << "SyntaxError: operator '" << sym 
//                      << "' missing left operand at position " << i << "\n";
//                 return;
//             }
//             if (!rightIsOperand) {
//                 cout << "SyntaxError: operator '" << sym 
//                      << "' missing right operand at position " << i << "\n";
//                 return;
//             }
//         }
//     }

//     // Check for unmatched parentheses
//     int openCount = 0;
//     for (auto& item : res.stack) {
//         if (item.symbol == "(") openCount++;
//         if (item.symbol == ")") openCount--;
//     }
//     if (openCount > 0) cout << "SyntaxError: unmatched '(' in input\n";
//     if (openCount < 0) cout << "SyntaxError: unmatched ')' in input\n";

//     // If nothing found, fall back to generic message
//     cout << "Unable to determine exact syntax error from stack.\n";
// }



