/* Declares the opcode dictionary */

#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

#include <iostream>
#include <unordered_map>

struct OpcodeInfo {
    int opcode;
    int format;
};

extern const std::unordered_map<std::string, OpcodeInfo> opcodeTable;
#endif // OPCODE_TABLE_H