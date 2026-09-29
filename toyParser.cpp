#include "toyTokenizer.cpp"
#include <map>

class Parser {
public:
    explicit Parser(const vector<Token>& sentence) : toks(sentence) {}

    bool parse() {
        parseS();
        if (ok && current().type != ENDFILE)
            error("end of line");
        return ok;
    }

private:
    vector<Token> toks;
    size_t pos = 0;
    bool ok = true;
    int depth = 0;

    const Token& current() const { return toks[pos]; }

    void trace(const string& msg) const {
        cout << "    " << string(depth * 2, ' ') << msg << "\n";
    }

    void error(const string& expected) {
        if (!ok) return;                        
        const Token& t = current();
        cout << "    Syntax error at line " << t.line << ", column " << t.column
             << ": expected " << expected << " but found '"
             << (t.type == ENDFILE ? "end of line" : t.lexeme) << "'\n";
        ok = false;
    }

    void match(TokenType expected) {
        if (current().type == expected) {
            trace("match '" + current().lexeme + "'");
            pos++;
        } else {
            error("'" + tokenTypeName(expected) + "'");
        }
    }

    void parseS() {
        trace("<S>"); depth++;
        parseA();
        if (ok && current().type == TK_C) {     
            parseC();
            if (ok) parseB();
        }                                       
        depth--;
    }

    void parseA() {
        trace("<A>"); depth++;
        match(TK_A);
        if (ok && current().type == TK_A) parseA();
        depth--;
    }

    void parseB() {
        trace("<B>"); depth++;
        match(TK_B);
        if (ok && current().type == TK_B) parseB();
        depth--;
    }

    void parseC() {
        trace("<C>"); depth++;
        match(TK_C);
        depth--;
    }
};

int main(int argc, char* argv[]) {
    string fileName = (argc > 1) ? argv[1] : "test1.txt";

    vector<Token> tokens = tokenize(fileName);
    if (tokens.empty()) return 1;

    cout << "===== Tokens in " << fileName << " =====\n";
    printTokens(tokens);

    map<int, vector<Token>> lines;
    for (const Token& t : tokens)
        if (t.type != ENDFILE) lines[t.line].push_back(t);

    cout << "\n===== Parsing " << fileName << " =====\n";
    for (auto& entry : lines) {
        vector<Token>& sentence = entry.second;
        string text;
        for (const Token& t : sentence) text += t.lexeme;
        sentence.push_back({ ENDFILE, "EOL", entry.first,
                             sentence.back().column + 1 });

        cout << "Line " << entry.first << ": \"" << text << "\"\n";
        Parser p(sentence);
        bool accepted = p.parse();
        cout << "  Result: " << (accepted ? "ACCEPTED" : "REJECTED") << "\n\n";
    }
    return 0;
}
