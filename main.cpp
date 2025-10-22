#include <iostream>
#include <iomanip>
#include <vector>
#include "lexer.h"
 
using namespace std;

struct TokenCounts {
    int identifiers = 0;
    int numbers = 0;
    int operators = 0;   // + - * /
    int assignment = 0;
    int parentheses = 0; // ( )
    int semicolons = 0;
    int invalids = 0;

    int total() const {
        return identifiers + numbers + operators + assignment + parentheses + semicolons + invalids;
    }
};


static void printTokenStream(const vector<Token>& tokens) {
    cout << left
         << setw(20) << "Token Type"
         << setw(20) << "Lexeme (Value)"
         << "Position\n";
    cout << string(55, '-') << "\n";

    for (const auto& t : tokens) {
        cout << left
             << setw(20) << tokenTypeName(t.type)
             << setw(20) << t.lexeme
             << t.position << "\n";
    }
    cout << "\n";
}


static TokenCounts countTokens(const vector<Token>& tokens) {
    TokenCounts c;
    for (const auto& t : tokens) {
        switch (t.type) {
            case TokenType::IDENTIFIER: c.identifiers++; break;
            case TokenType::NUMBER:     c.numbers++;     break;
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::MUL:
            case TokenType::DIV:        c.operators++;   break;
            case TokenType::ASSIGN:     c.assignment++;  break;
            case TokenType::LPAREN:
            case TokenType::RPAREN:     c.parentheses++; break;
            case TokenType::SEMICOLON:  c.semicolons++;  break;
            case TokenType::INVALID:    c.invalids++;    break;
        }
    }
    return c;
}

static void printCounts(const TokenCounts& c) {
    cout << "Token Counts:\n----------------------\n";
    cout << "  Identifiers : " << c.identifiers  << "\n";
    cout << "  Numbers     : " << c.numbers      << "\n";
    cout << "  Operators   : " << c.operators    << "\n";
    cout << "  Assignment  : " << c.assignment   << "\n";
    cout << "  Parentheses : " << c.parentheses  << "\n";
    cout << "  Semicolons  : " << c.semicolons   << "\n";
    cout << "  Invalid     : " << c.invalids     << "\n";
    cout << "----------------------\n";
    cout << "  Total       : " << c.total()      << "\n\n";
}

static void printLexicalErrors(const vector<LexicalError>& errors) {
    if (errors.empty()) return;
    cout << "Lexical Errors:\n";
    for (const auto& e : errors) {
        cout << "  LexicalError at position " << e.position << ": " << e.message << "\n";
    }
    cout << "\n";
}

static void runOnce(const string& input) {
    vector<LexicalError> errors;
    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize(errors);

    cout << "Input: " << input << "\n\n";
    printTokenStream(tokens);
    TokenCounts counts = countTokens(tokens);
    printCounts(counts);
    printLexicalErrors(errors);
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);

    // Batch mode if "--batch" is provided //easier for video record
    if (argc >= 2 && string(argv[1]) == "--batch") {
        vector<string> cases = {
            "x = y + 3;",
            "a = (b + c) * d;",
            "total = m / (n - p);",
            "x = y + ;",
            "num = 5.5;"
        };

        cout << "=== COMPY Lexer: Batch Mode ===\n\n";
        int caseNo = 1;
        for (const auto& s : cases) {
            cout << "--------------------- Test Case " << caseNo++ << " ---------------------\n";
            runOnce(s);
        }
        cout << "============== End of Batch ==============\n";
        return 0;
    }


    // single-line mode
    cout << "Enter a single-line COMPY statement (e.g., x = (3 + 5) * 2;):\n" << endl;
    string line;
    if (!getline(cin, line)) return 0;
    runOnce(line);

    return 0;
}
