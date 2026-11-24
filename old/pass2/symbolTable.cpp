#include "symbolTable.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool loadSymbolTable(
    const string& filename,
    unordered_map<string, int>& symbolTable
) {
    // opens the file for reading the symbol table
    ifstream file (filename);

    // error if error encountered
    if (!file.is_open()) {
        cerr << "Error, could not open " << filename << "\n";
        return false;
    }

    string line;

    // read each line for the information in the symbol table
    while(getline(file, line)){
        if (line.empty()){
            continue;
        }

        string symbol;
        string hexAddress;

        istringstream iss(line);

        if (!(iss >> symbol >> hexAddress)) {
            cerr << "Error, invalid line format: " << line << "\n";
            continue;
        }

        try {
            int address = stoi(hexAddress, nullptr, 16); 
            symbolTable[symbol] = address;
        } catch (const exception&) {
            cerr << "Error: Invalid address. " << hexAddress << "\n";
        }
    }

    return true;
    
}