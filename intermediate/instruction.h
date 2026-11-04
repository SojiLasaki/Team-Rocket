#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

struct Instruction {
    std::string symbol;
    std::string opCode;
    std::string opCodeHexNumber;
    std::string operand;
    std::string operandHexNumber;

    int n = 0;
    int i = 0;
    int x = 0;
    int b = 0;
    int p = 0;
    int e = 0;

    int locationCounter = 0;
    int programCounter = 0;
    int targetFeild = 0;
    std::string objectCode;
    int format = 0;
};

#endif // INSTRUCTION_H