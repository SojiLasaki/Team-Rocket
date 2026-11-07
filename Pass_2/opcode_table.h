#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

#include <string>
#include <unordered_map>

struct OpcodeInfo {
    int opcode;
    int format;
};

extern std::unordered_map<std::string, OpcodeInfo> opcodeTable;


#endif // OPCODE_TABLE_H