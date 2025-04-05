/*
1. Compiler Design
Assignment: Write a Simple Lexer in Python
Task: Create a Python program that tokenizes an arithmetic expression. For example, given the input "3 + 5 * (2 - 8)", your lexer should output a list of tokens like [3, "+", 5, "*", "(", 2, "-", 8, ")"]. Tokens should include numbers, operators (+, -, *, /), and parentheses.
Instructions: Use string manipulation or regular expressions to identify token types. Test your lexer with at least three different expressions.
Objective: Understand the first step of compilation—breaking code into manageable pieces.
Tools: Python (no external libraries required yet).
*/


#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    vector<string> tokens;
    string input;
    cout << "Enter a String\n";
    getline(cin, input);
    string currentnumber = "";
    bool hasError = 0;
    string errMessage = "";
    for (int i = 0; i < input.size(); i++) {
        char c = input[i];
        if (isdigit(c)) {
            currentnumber += c;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            if (!currentnumber.empty()) {
                tokens.push_back(currentnumber);
                currentnumber = "";
            }
            tokens.push_back(string(1, c));
        }
        else if (c == ' ') {
            if (!currentnumber.empty()) {
                tokens.push_back(currentnumber);
                currentnumber = "";
            }
        }
        else {
            hasError = 1;
            errMessage = "Invalid Character '" + string(1, c) + "' at position " + to_string(i + 1);
        }
    }
    if (!currentnumber.empty()) {
        tokens.push_back(currentnumber);
        currentnumber = "";
    }
    if (hasError) {
        cout << errMessage;
    }
    else {
        for (int i = 0; i < tokens.size(); i++) {
            cout << tokens[i] << " \n";
        }
    }
    return 0;
}