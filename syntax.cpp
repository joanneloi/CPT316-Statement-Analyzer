#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class Parser {
private:
    vector<string> tokens;
    int pos;

    string current_token() {
        if (pos < tokens.size())
            return tokens[pos];
        return "EOF";
    }

    string match_token(const string& expected) {
        if (current_token() == expected) {
            string matched = current_token();
            pos++;
            return matched;
        } else {
            throw runtime_error("Syntax error: expected '" + expected + "' but found '" + current_token() + "'");
        }
    }

public:
    Parser(vector<string> tokenStream) {
        tokens = tokenStream;
        pos = 0;
    }

    // <stmt> → id assign <expr> semi
    void stmt(int indent = 0) {
        printIndent(indent); 
        cout << "stmt" << endl;
        match_token("id");
        printIndent(indent + 1); 
        cout << "id" << endl;

        match_token("assign");
        printIndent(indent + 1); 
        cout << "assign" << endl;

        expr(indent + 1);

        match_token("semi");
        printIndent(indent + 1); 
        cout << "semi" << endl;
    }

    // <expr> → id | id op int
    void expr(int indent = 0) {
        printIndent(indent); 
        cout << "expr" << endl;
        match_token("id");
        printIndent(indent + 1); 
        cout << "id" << endl;

        if (current_token() == "op") {
            match_token("op");
            printIndent(indent + 1); 
            cout << "op" << endl;
            match_token("int");
            printIndent(indent + 1); 
            cout << "int" << endl;
        }
    }

    void parse() {
        stmt();
        if (pos < tokens.size()) {
            throw runtime_error("Extra tokens after valid statement");
        }
    }

    void printIndent(int indent) {
        for (int i = 0; i < indent; i++) cout << "  ";
    }
};

int main() {
    try {
        // Example valid input: id assign id op int semi  (represents x = y + 3;)
        string input = "id,assign,id,op,int,semi";

        // Split the input string into tokens
        vector<string> tokens;
        string temp = "";
        for (char c : input) {
            if (c == ',') {
                tokens.push_back(temp);
                temp = "";
            } else {
                temp += c;
            }
        }
        if (!temp.empty()) tokens.push_back(temp);

        Parser parser(tokens);
        cout << "✅ Syntax Tree:" << endl;
        parser.parse();
    }
    catch (const exception& e) {
        cerr << "❌ " << e.what() << endl;
    }

    return 0;
}
