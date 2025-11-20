#include <iostream>
#include <iomanip>
#include <vector>
#include "lexer.h"
#include "parser.h"
using namespace std;

static void runOnce(const string& input) {
    vector<LexicalError> errors;
    Lexer lexer(input);
    vector<Token> tokens;
    // Scan and tokenize the input
    tokens = lexer.tokenize(errors);

    cout << "Input: " << input << "\n\n";

    // 1. Print tokens
    printTokenStream(tokens);
    TokenCounts counts = countTokens(tokens);
    printCounts(counts);

    // 2. If lexical errors exist → print the error and do not parse
    if (!errors.empty()) {
        printLexicalErrors(errors);
        cout << "\nSkipping parser because lexical errors were found.\n";
        return;
    }

    // 3. Run the Parser
    cout << "\n=== Parsing Result ===\n";
    Parser parser(tokens);

    parser.parse();
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    // single-line mode
    cout << "Enter a single-line COMPY statement (e.g., x = (3 + 5) * 2;):\n" << endl;
    string line;
    if (!getline(cin, line)) return 0;
    runOnce(line);

    return 0;
}
