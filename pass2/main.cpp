
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#include "opcode_table.h"

using namespace std;

/* Instance of an instruction */
struct Instruction {
    string symbol;
    string opcode;
    string operand;

    int ni = 0;
    int x = 0;
    int b = 0;
    int p = 0;
    int e = 0;

    int locctr = 0;
    int programCtr = 0;
    int address = 0;
};

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
    for (const Instruction& inst : instructions) {
        auto it = opcodeTable.find(inst.opcode);

        if (it != opcodeTable.end()) {
            int machineOpcode = it->second.opcode;
            int format = it->second.format;

            cout << "Instruction: " << inst.opcode << '\n';

            cout << "Machine opcode: 0x"
                 << hex << uppercase
                 << machineOpcode << dec << '\n';

            cout << "Default format: "
                 << format << '\n';

            cout << "Instruction length: "
                 << format << " bytes\n";
        } else {
            cout << "Unknown opcode: "
                 << inst.opcode << '\n';
        }

        cout << "-------------------\n";
    }

    return 0;
}
