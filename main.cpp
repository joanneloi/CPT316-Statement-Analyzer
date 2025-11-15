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
    printLexicalErrors(errors);

    // 2. If lexical errors exist → do not parse
    if (!errors.empty()) {
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
