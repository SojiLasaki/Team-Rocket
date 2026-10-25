
#include <iostream>
#include <vector>
#include <string>
#include "instruction.h"
#include "opcode_table.h"
#include <iomanip>
#include "symbolTable.h"
#include <unordered_map>
#include "opcode_table.h"

using namespace std;

int main() {
    vector<Instruction> instructions;

    // Create a sample instruction
    Instruction first;
    first.symbol = "FIRST";
    first.opcode = "LDA";
    first.operand = "VALUE";
    first.ni = 3;
    first.locctr = 0;

    instructions.push_back(first);

    // Look up opcode and format
    for (Instruction& inst : instructions) {
        auto it = opcodeTable.find(inst.opcode);

        if (it != opcodeTable.end()) {
            inst.machineOpcode = it->second.opcode;
            inst.format = it->second.format;

            cout << "Instruction: " << inst.opcode << '\n';hha
            cout << "Machine opcode: 0x" << hex << uppercase << inst.machineOpcode << dec << '\n';
            cout << "Fomrat: " << inst.format << '\n';
        } else {
            cout << "Unknown opcode " << inst.opcode << '\n';           
        }
    }

    return 0;
}
