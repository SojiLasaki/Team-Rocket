#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

struct Instruction {
    std::string symbol;
    std::string opcode;
    std::string operand;

    int ni = 0;
    int x = 0;
    int b = 0;
    int p = 0;
    int e = 0;

    int locctr = 0;
    int programCtr = 0;
    int address = 0;

    int machineOpcode = 0;
    int format = 0;
};

#endif // INSTRUCTION_H