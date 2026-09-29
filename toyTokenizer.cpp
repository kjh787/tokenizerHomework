#ifndef TOY_TOKENIZER_CPP
#define TOY_TOKENIZER_CPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;


enum TokenType { TK_A, TK_B, TK_C, ENDFILE, UNKNOWN };

struct Token {
    TokenType type;
    string    lexeme;
    int       line;
    int       column;
};


string tokenTypeName(TokenType type) {
    switch (type) {
        case TK_A:    return "a";
        case TK_B:    return "b";
        case TK_C:    return "c";
        case ENDFILE: return "ENDFILE";
        default:      return "UNKNOWN";
    }
}

TokenType lookupKeyword(char ch) {
    switch (ch) {
        case 'a': return TK_A;
        case 'b': return TK_B;
        case 'c': return TK_C;
        default:  return UNKNOWN;
    }
}

vector<Token> tokenize(const string& fileName) {
    vector<Token> tokens;
    ifstream in(fileName);
    if (!in) {
        cerr << "Error: cannot open file '" << fileName << "'\n";
        return tokens;
    }

    int line = 1, column = 0;
    char ch;
    while (in.get(ch)) {
        if (ch == '\n') { line++; column = 0; continue; }
        if (ch == '\r') continue;              
        column++;
        tokens.push_back({ lookupKeyword(ch), string(1, ch), line, column });
    }
    tokens.push_back({ ENDFILE, "EOF", line, column + 1 });
    return tokens;
}

void printTokens(const vector<Token>& tokens) {
    cout << left << setw(10) << "TOKEN" << setw(8) << "LEXEME"
         << setw(6) << "LINE" << "COLUMN\n";
    cout << "--------------------------------\n";
    for (const Token& t : tokens) {
        cout << left << setw(10) << tokenTypeName(t.type) << setw(8) << t.lexeme
             << setw(6) << t.line << t.column << "\n";
    }
}

#endif
