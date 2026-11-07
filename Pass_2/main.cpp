#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// open and load the json file
int main() {
    ifstream inputFile("test.json");
    if(!inputFile.is_open()) {
        cout << "Failed to open input file." << endl;
        return 1;
    }
    cout << "Input file opened successfully." << endl;

    json instructions;
    inputFile >> instructions;
    inputFile.close();

    // print the loaded instructions
    for (const auto& instruction : instructions) {
        string format = instruction["format"];
        string n = instruction["n"];
        string i = instruction["i"];
        string x = instruction["x"];
        string b = instruction["b"];
        string p = instruction["p"];
        string e = instruction["e"];

        cout << "Format: " << format << endl;
        cout << "N: " << n << endl;
        cout << "I: " << i << endl;
        cout << "X: " << x << endl;
        cout << "B: " << b << endl;
        cout << "P: " << p << endl;
        cout << "E: " << e << endl;
        cout << "-----------------" << endl;
        
        // checking for format 1 instruction
        if (format == "1") {
            cout << "Format 1 instruction" << endl;
        }

        // checking for format 2 instruction
        else if (format == "2") {
            cout << "Format 2 instruction" << endl;
        }

        // checking for format 3 instruction
        else if (format == "3") {
            cout << "Format 3 instruction" << endl;

            // check the flags
            if (e != "0") {
                cout << "Invalid format 3 instruction flag E" << endl;
            }
            if (b ==  "1" && p == "1") {
                cout << "Invalid format 3 instruction with both B and P bits set" << endl;
            }

            // formating the opcode and N I flags
            int opcodeValue = stoi(instruction["opCode"].get<string>(), nullptr, 16);

            int firstByte = (opcodeValue & 0xFC) | (stoi(n) << 1) | stoi(i);

            // check flags xbpe and assign shifts to positioins
            // convert from string to integer for target field
            int targetValue = stoi(instruction["targetField"].get<string>(), nullptr, 16);

            int secondByte = (stoi(x) << 7) |
                             (stoi(b) << 6) |
                             (stoi(p) << 5) |
                             (stoi(e) << 4) |
                             ((targetValue >> 8) & 0x0F);

            cout << "First byte: " << hex << uppercase << firstByte << endl;
        }

        int firstByte = (opcodeValue & 0xFC) |
                        (stoi(n) << 1) |
                        stoi(1);

        int thiedNye = targetValue & 0xFF;

        // checking for format 4 instruction
        else if (format == "4") {
            cout << "Format 4 instruction" << endl;
            if (e != "1") {
                cout << "Invalid format 4 instruction flag E" << endl;
            }
            if (b ==  "1" || p == "1") {
                cout << "Invalid format 4 instruction with both B and P bits set" << endl;
            }
        }

        else {
            cout << "Unknown format instruction" << endl;
        }
    }
    return 0;
}

