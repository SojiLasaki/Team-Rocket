// SIC/XE Assembler - Pass 1 (line parser)
// Reads a SIC/XE source file, drops comments and blank lines, and writes one
// record per real instruction/directive with symbol, opCode and operand filled in.
//
// Build: g++ -std=c++11 -o pass1 pass1.cpp
// Run:   ./pass1 source.asm output.txt

#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
using namespace std;

// Opcodes that never take an operand (anything after them is a comment)
static const set<string> NO_OPERAND = {
    "RSUB", "FIX", "FLOAT", "HIO", "NORM", "SIO", "TIO", "NOBASE"};

struct Instruction {
    string symbol;
    string opCode;
    string opCodeHexNumber;
    string operand;
    string operandHexNumber;

    int n = 0;
    int i = 0;
    int x = 0;
    int b = 0;
    int p = 0;
    int e = 0;

    string locationCounter = "0";
    string programCounter = "0";
    string targetFeild = "0";
    string objectCode = "0";
    int format = 0;
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
static bool parseLine(string line, Instruction &rec) {
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

static void writeRecord(ofstream &out, const Instruction &r) {
    out << "{\"symbol\" : \"" << r.symbol << "\", "
        << "\"opCode\" : \"" << r.opCode << "\", "
        << "\"opCodeHexNumber\" : \"" << r.opCodeHexNumber << "\", "
        << "\"operand\" : \"" << r.operand << "\", "
        << "\"operandHexNumber\" : \"" << r.operandHexNumber << "\", "
        << "\"n\" : \"" << r.n << "\", "
        << "\"i\" : \"" << r.i << "\", "
        << "\"x\" : \"" << r.x << "\", "
        << "\"b\" : \"" << r.b << "\", "
        << "\"p\" : \"" << r.p << "\", "
        << "\"e\" : \"" << r.e << "\", \n"
        << "\"locationCounter\" : \"" << r.locationCounter << "\", "
        << "\"programCounter\" : \"" << r.programCounter << "\", "
        << "\"targetFeild\" : \"" << r.targetFeild << "\", "
        << "\"objectCode\" : \"" << r.objectCode << "\", "
        << "\"format\" : \"" << r.format << "\" },\n\n";
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

    string fileText((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    string line;
    int count = 0;
    auto handleLine = [&](const string &text) {
        Instruction rec;
        if (parseLine(text, rec)) {
            writeRecord(out, rec);
            count++;
        }
    };
    for (size_t k = 0; k < fileText.size(); k++) {
        char c = fileText[k];
        if (c == '\r' || c == '\n') {
            handleLine(line);
            line.clear();
            if (c == '\r' && k + 1 < fileText.size() && fileText[k + 1] == '\n') k++;
        } else {
            line += c;
        }
    }
    handleLine(line);

    cout << "Wrote " << count << " records to " << outName << "\n";
    return 0;
}