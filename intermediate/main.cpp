#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <cctype>
#include <cmath>

#include "helpers.h"
#include "../pass2/opcode_table.cpp"

void getUpperCaseString(std::string str)
{
}

std::unordered_map<std::string, std::string> symbolTable;

bool isSymbolUnique(std::string symbolName)
{
    auto symbol = symbolTable.find(symbolName);

    if (symbol != symbolTable.end())
    {
        std::cout << "Error Symbol: " << symbolName << " is not unique" << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}
void addSymbol(std::string symbolName, std::string locationCounter)
{

    // std::cout << symbolName << std::endl;
    symbolTable.insert({symbolName, locationCounter});
}

int main()
{

    std::string hex;
    std::string res;

    std::string locationCounter;
    std::string startAddress;
    // Pass 2 Start
    struct Items
    {

    public:
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

    Items item1 = Items{"start", "LDA", "2B"};
    Items item2 = Items{"first", "LDB", "#008"};
    Items item3 = Items{"incr", "LDT", "#300"};
    Items item4 = Items{"AddLp", "LDX", "#0"};
    std::vector<Items> vecItems;

    vecItems.push_back(item1);
    vecItems.push_back(item2);
    vecItems.push_back(item3);
    vecItems.push_back(item4);
    Items firstItem = vecItems[0];
    std::string firstItemSym = vecItems[0].symbol;
    std::transform(firstItemSym.begin(), firstItemSym.end(), firstItemSym.begin(), [](unsigned char c)
                   { return std::toupper(c); });

    if (firstItemSym == "START")
    {
        startAddress = firstItem.operand;
        locationCounter = firstItem.operand;
        std::cout << "start address:" << startAddress << " Location Counter : " << locationCounter << std::endl;
    }
    else
    {
        locationCounter = "0";
        startAddress = "0";
        std::cout << "start address:" << startAddress << " Location Counter : " << locationCounter << std::endl;
    }

    std::vector<Items> outPutItems;
    int i = 0;

    for (Items item : vecItems)
    {
        std::string outputSym;
        std::string outputOpCode;
        std::string outputLocCtr;
        std::string outputFormat;
        int outputN;
        int outputI;
        int outputX;
        int outputB;
        int outputP;
        int outputE;

        // 1.)Write Symbol and locctr to table
        if (item.symbol != "")
        {
            if (isSymbolUnique(item.symbol))
            {
                addSymbol(item.symbol, locationCounter);
                // outPutItems[i].symbol = item.symbol;
                // outPutItems[i].locationCounter = locationCounter;
            }
            else
            {
                continue;
            }

            // 2.) Detetct OpCode
            if (item.opCode != "")
            {
                auto opCodeFormat = opcodeTable.find(item.opCode);
                if (opCodeFormat != opcodeTable.end())
                {
                    if (item.symbol == "")
                    {
                        outputLocCtr = locationCounter;

                        // switch (opCodeFormat.first) {

                        // case "RESB";
                        // if (item.operand != ""){

                        //     std::
                        //     locationCounter += item.operand;
                        // }
                        // break;

                        // case "RESW"
                        // if (item.operand != ""){
                        //     std::string valueToEncode =
                        //     locationCounter += 3 * item
                        // }
                    }
                }
                std::cout << "Op Code available " << item.opCode << std::endl;
            }
        }
    }

    //  ///1.) Print Results
    for (auto it : symbolTable)
    {
        std::cout << " Symbol Value " << it.first << " " << it.second << std::endl;
    }

    // Hex	Decimal
    // 0A	10
    // 10	16
    // 1B	27
    // 2F	47
    // 7F	127
    // FF	255
    // 6060	24,672
    // 2F83	12,163

    std::string val = computeTwoHexValues("1A2B0", "03C70", ADD);

    std::cout << "Return Computed Two Hex" << val;

    return 0;
}