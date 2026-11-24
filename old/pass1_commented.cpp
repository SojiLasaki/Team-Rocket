// SIC/XE Assembler - Pass 1
// writes symbol, opcode, and operand for each real line of code

// isspace, toupper
#include <cctype>
// file reading/writing
#include <fstream>
// cout, cin, cerr
#include <iostream>
// set
#include <set>
// string
#include <string>
// skip typing std::
using namespace std;

// opcodes with no operand
static const set<string> NO_OPERAND = {
    // rest of the line after these is a comment
    "RSUB", "FIX", "FLOAT", "HIO", "NORM", "SIO", "TIO", "NOBASE"};

// one line of code
struct Record {
    // label
    string symbol;
    // instruction or directive
    string opCode;
    // operand
    string operand;
    // n and i bits
    int n = 0, i = 0;
    // x, b, p, e flags
    int x = 0, b = 0, p = 0, e = 0;
    // location counter
    int locctr = 0;
    // program counter
    int programCtr = 0;
    // address
    int address = 0;
// end of Record
};

// makes a string uppercase
static string upper(string s) {
    // uppercase each character
    for (char &c : s) c = toupper(static_cast<unsigned char>(c));
    // return the result
    return s;
}

// moves i past spaces and tabs
static void skipSpaces(const string &s, size_t &i) {
    // advance while the character is whitespace
    while (i < s.size() && isspace(static_cast<unsigned char>(s[i]))) i++;
}

// reads one word starting at i
static string readToken(const string &s, size_t &i) {
    // skip leading spaces
    skipSpaces(s, i);
    // word starts here
    size_t start = i;
    // advance to the next space or end of line
    while (i < s.size() && !isspace(static_cast<unsigned char>(s[i]))) i++;
    // return the word
    return s.substr(start, i - start);
}

// reads the operand starting at i
static string readOperand(const string &s, size_t &i) {
    // skip leading spaces
    skipSpaces(s, i);
    // operand being built
    string op;
    // true between single quotes
    bool inQuote = false;
    // go through the rest of the line
    while (i < s.size()) {
        // current character
        char c = s[i];
        // toggle on each single quote
        if (c == '\'') inQuote = !inQuote;
        // stop at a space outside quotes
        if (!inQuote && isspace(static_cast<unsigned char>(c))) break;
        // add the character
        op += c;
        // next character
        i++;
    // end of loop
    }
    // ends in comma, so add the next word (BUFFER, X)
    if (!op.empty() && op.back() == ',') op += readToken(s, i);
    // return the operand
    return op;
}

// splits a line into symbol, opcode, and operand
// returns false for comment or blank lines
static bool parseLine(string line, Record &rec) {
    // remove trailing \r and \n
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
        // remove last character
        line.pop_back();

    // position in the line
    size_t i = 0;
    // go to first non-whitespace character
    skipSpaces(line, i);
    // only whitespace means blank line
    if (i >= line.size()) return false;  // blank
    // starts with . means comment
    if (line[i] == '.') return false;    // comment

    // no leading whitespace means there is a label
    bool hasLabel = !isspace(static_cast<unsigned char>(line[0]));
    // back to the start
    i = 0;

    // first word on the line
    string first = readToken(line, i);
    // line has a label
    if (hasLabel) {
        // copy of i to peek ahead
        size_t j = i;
        // a lone RSUB-type word is an opcode
        if (readToken(line, j).empty() && NO_OPERAND.count(upper(first))) {
            // save as opcode
            rec.opCode = first;
            // valid line
            return true;
        // end of check
        }
        // first word is the label
        rec.symbol = first;
        // next word is the opcode
        rec.opCode = readToken(line, i);
    // end of label case
    } else {
        // first word is the opcode
        rec.opCode = first;
    // end of no-label case
    }

    // no opcode means skip the line
    if (rec.opCode.empty()) return false;  // label only

    // opcode takes an operand
    if (!NO_OPERAND.count(upper(rec.opCode)))
        // read the operand
        rec.operand = readOperand(line, i);
    // rest of the line is a comment and is ignored
    // valid line
    return true;
}

// writes one record to the output file
static void writeRecord(ofstream &out, const Record &r) {
    // open bracket
    out << "{\n"
        // symbol
        << "\tsymbol : " << r.symbol << ",\n"
        // opcode
        << "\topCode = " << r.opCode << ",\n"
        // operand
        << "\toperand : " << r.operand << ",\n"
        // n and i
        << "\tn : " << r.n << ", i : " << r.i << ",\n"
        // x, b, p, e
        << "\tx : " << r.x << ", b : " << r.b << ", p : " << r.p
        << ", e : " << r.e << ",\n"
        // locctr
        << "\tlocctr : " << r.locctr << "\n"
        // programCtr
        << "\tprogramCtr : " << r.programCtr << "\n"
        // address
        << "\taddress : " << r.address << "\n"
        // close bracket
        << "},\n";
}

// main
int main(int argc, char *argv[]) {
    // input and output file names
    string inName, outName;
    // filename given on the command line
    if (argc >= 2) {
        // use it as input
        inName = argv[1];
    // end of if
    } else {
        // ask for the input file
        cout << "Input SIC/XE file: ";
        // read the name
        getline(cin, inName);
    // end of else
    }
    // second argument is the output file, default pass1_output.txt
    outName = (argc >= 3) ? argv[2] : "pass1_output.txt";

    // open input file
    ifstream in(inName);
    // input failed to open
    if (!in) {
        // print error
        cerr << "Error: cannot open " << inName << "\n";
        // exit with error
        return 1;
    // end of if
    }
    // open output file
    ofstream out(outName);
    // output failed to open
    if (!out) {
        // print error
        cerr << "Error: cannot create " << outName << "\n";
        // exit with error
        return 1;
    // end of if
    }

    // current line
    string line;
    // records written
    int count = 0;
    // read each line
    while (getline(in, line)) {
        // empty record
        Record rec;
        // line is real code
        if (parseLine(line, rec)) {
            // write it
            writeRecord(out, rec);
            // count it
            count++;
        // end of if
        }
    // end of loop
    }

    // print the total
    cout << "Wrote " << count << " records to " << outName << "\n";
    // exit with no error
    return 0;
}
