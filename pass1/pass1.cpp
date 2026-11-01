// SIC/XE Assembler - Pass 1 (line parser)
// Reads a SIC/XE source file, drops comments and blank lines, and writes one
// record per real instruction/directive with symbol, opCode and operand filled in.
//
// Build: g++ -std=c++11 -o pass1 pass1.cpp
// Run:   ./pass1 source.asm output.txt

#include <cctype>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
using namespace std;

// Opcodes that never take an operand (anything after them is a comment)
static const set<string> NO_OPERAND = {
    "RSUB", "FIX", "FLOAT", "HIO", "NORM", "SIO", "TIO", "NOBASE"};

struct Record {
    string symbol;
    string opCode;
    string operand;
    int n = 0, i = 0;
    int x = 0, b = 0, p = 0, e = 0;
    int locctr = 0;
    int programCtr = 0;
    int address = 0;
};

static string upper(string s) {
    for (char &c : s)
        c = toupper(static_cast<unsigned char>(c));
    return s;
}

static void skipSpaces(const string &s, size_t &i) {
    while (i < s.size() &&
           isspace(static_cast<unsigned char>(s[i])))
        i++;
}

// Reads one whitespace-delimited word (empty string if none left)
static string readToken(const string &s, size_t &i) {
    skipSpaces(s, i);
    size_t start = i;
    while (i < s.size() &&
           !isspace(static_cast<unsigned char>(s[i])))
        i++;
    return s.substr(start, i - start);
}

// Reads an operand. Spaces inside quotes (C'HELLO WORLD') are kept, and an
// operand like "BUFFER, X" (space after comma) is joined into one operand.
static string readOperand(const string &s, size_t &i) {
    skipSpaces(s, i);
    string op;
    bool inQuote = false;
    while (i < s.size()) {
        char c = s[i];
        if (c == '\'') inQuote = !inQuote;
        if (!inQuote && isspace(static_cast<unsigned char>(c))) break;
        op += c;
        i++;
    }
    if (!op.empty() && op.back() == ',')
        op += readToken(s, i);
    return op;
}

// Returns false if the line is a comment/blank (nothing to translate)
static bool parseLine(string line, Record &rec) {
    while (!line.empty() &&
           (line.back() == '\r' || line.back() == '\n'))
        line.pop_back();

    size_t i = 0;
    skipSpaces(line, i);
    if (i >= line.size()) return false;  // blank line
    if (line[i] == '.') return false;    // comment line

    bool hasLabel = !isspace(static_cast<unsigned char>(line[0]));
    i = 0;

    string first = readToken(line, i);
    if (hasLabel) {
        // A lone word like "RSUB" in column 1 is an opcode, not a label
        size_t j = i;
        if (readToken(line, j).empty() &&
            NO_OPERAND.count(upper(first))) {
            rec.opCode = first;
            return true;
        }
        rec.symbol = first;
        rec.opCode = readToken(line, i);
    } else {
        rec.opCode = first;
    }

    if (rec.opCode.empty()) return false;  // label with nothing else

    if (!NO_OPERAND.count(upper(rec.opCode)))
        rec.operand = readOperand(line, i);
    // anything left on the line is a trailing comment and is ignored
    return true;
}

static void writeRecord(ofstream &out, const Record &r) {
    out << "{\n"
               << "\tsymbol : " << r.symbol << ",\n"
               << "\topCode = " << r.opCode << ",\n"
               << "\toperand : " << r.operand << ",\n"
               << "\tn : " << r.n << ", i : " << r.i << ",\n"
               << "\tx : " << r.x << ", b : " << r.b
               << ", p : " << r.p << ", e : " << r.e << ",\n"
               << "\tlocctr : " << r.locctr << "\n"
               << "\tprogramCtr : " << r.programCtr << "\n"
               << "\taddress : " << r.address << "\n"
               << "},\n";
}

int main(int argc, char *argv[]) {
    string inName, outName;
    if (argc >= 2) {
        inName = argv[1];
    } else {
        cout << "Input SIC/XE file: ";
        getline(cin, inName);
    }
    outName = (argc >= 3) ? argv[2] : "pass1_output.txt";

    ifstream in(inName);
    if (!in) {
        cerr << "Error: cannot open " << inName << "\n";
        return 1;
    }
    ofstream out(outName);
    if (!out) {
        cerr << "Error: cannot create " << outName << "\n";
        return 1;
    }

    string line;
    int count = 0;
    while (getline(in, line)) {
        Record rec;
        if (parseLine(line, rec)) {
            writeRecord(out, rec);
            count++;
        }
    }

    cout << "Wrote " << count << " records to " << outName << "\n";
    return 0;
}
