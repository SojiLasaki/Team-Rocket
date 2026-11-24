#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>

using namespace std;

// function returns true if the files were loaded properly
bool loadSYmbolTable(
    const string& filename
    // this stores the values from the file into a table
    unordered_map<string, int>& symbolTable
);

#endif 