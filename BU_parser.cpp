#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include "BU_parser.h"
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

// bool Parser::parse() {
//     cout << "=== SHIFT-REDUCE PARSING START ===\n";

//     while (true) {
//         bool reduced = tryReduce();
//         if (!reduced) {
//             if (tokens[pos].type == TokenType::END &&
//                 stack.size() == 1 && stack.back().symbol == "<stmt>") {
//                 cout << "\n✅ Input accepted.\n\nParse Tree:\n";
//                 printTree(stack.back().node);
//                 return true; 
//             }
//             if (tokens[pos].type == TokenType::END) {
//                 cout << "\n❌ Syntax error: unexpected end of input.\n";
//                 return false; 
//             }
//             shift();
//         }
//     }
// }

// bool Parser::parse() {
//     cout << "=== SHIFT-REDUCE PARSING START ===\n";

//     while (true) {
//         // Keep trying to reduce as long as possible
//         bool reduced = tryReduce();

//         // ✅ Acceptance condition
//         if (tokens[pos].type == TokenType::END &&
//             stack.size() == 1 &&
//             stack.back().symbol == "<stmt>") {
//             cout << "\n✅ Input accepted.\n\nParse Tree:\n";
//             printTree(stack.back().node);
//             return true;
//         }

//         // ✅ When no more reductions are possible:
//         if (!reduced) {
//             // If we’ve reached the end, but cannot reduce further → reject
//             if (tokens[pos].type == TokenType::END) {
//                 cout << "\n❌ Syntax error: unexpected end of input.\n";
//                 return false;
//             }

//             // Otherwise, shift next token
//             shift();
//         }
//     }
// }

bool Parser::parse() {
    cout << "=== SHIFT-REDUCE PARSING START ===\n";

    while (true) {
        // 1. CHECK ACCEPTANCE at the start of the loop
        if (tokens[pos].type == TokenType::END &&
            stack.size() == 1 &&
            stack.back().symbol == "<stmt>") {
            
            cout << "\n✅ Input accepted.\n\nParse Tree:\n";
            printTree(stack.back().node);
            return true;
        }

        // 2. Try to Reduce
        bool reduced = tryReduce();

        if (!reduced) {
            // 3. If no reduction, check for END
            if (tokens[pos].type == TokenType::END) {
                cout << "\n❌ Syntax error: unexpected end of input.\n";
                return false;
            }

            // 4. Otherwise, Shift
            shift();
        }
        // If reduced was true, the loop restarts and hits the acceptance check.
    }
}


// void Parser::shift() {
//     Token t = tokens[pos++];
//     Node* node = new Node(tokenTypeName(t.type));
//     stack.push_back({tokenTypeName(t.type), node});
//     cout << "Shift: " << tokenTypeName(t.type) << "\n";
// }

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
            // ⚠️ Prevent premature reduction of 'id' into <factor> when next token is '='
            if (pos < tokens.size()) {
                string next = tokens[pos].lexeme;
                // ⚠️ Prevent reducing <term> → <factor> if next is * or /
                if (rhs == vector<string>{"<term>"} && (next == "*" || next == "/"))
                    continue;
                // ⚠️ Prevent reducing id → <factor> when followed by '=' (for assignment)
                if (rhs == vector<string>{"id"} && next == "=")
                    continue;
            }

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
};