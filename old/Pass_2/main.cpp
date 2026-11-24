#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;
using json = nlohmann::json;

// open and load the json file
int main() {
    ifstream inputFile("../intermediate/BasicsTxtIntermediateFile.json");
    if(!inputFile.is_open()) {
        cout << "Failed to open input file." << endl;
        return 1;
    }
    cout << "Input file opened successfully." << endl;

    json instructions;
    inputFile >> instructions;
    inputFile.close();

    // Pass 2 output is written as standard SIC/XE H/T/E records.
    auto hexNumber = [](const json& value, const char* field) -> unsigned long {
        try {
            string text = value.is_string() ? value.get<string>() : value.dump();
            return stoul(text, nullptr, 16);
        } catch (...) {
            throw runtime_error(string("Invalid hexadecimal field '") + field + "'");
        }
    };
    auto paddedHex = [](unsigned long value, int width) {
        ostringstream out;
        out << uppercase << hex << setfill('0') << setw(width) << value;
        return out.str();
    };

    if (!instructions.is_array() || instructions.empty()) {
        cerr << "Input JSON must be a non-empty array of instruction records." << endl;
        return 1;
    }

    try {
        unsigned long start = hexNumber(instructions.front().at("locationCounter"), "locationCounter");
        unsigned long end = start;
        string programName = instructions.front().value("programName", instructions.front().value("symbol", string("NONAME")));
        if (programName.empty()) programName = "NONAME";
        if (programName.size() > 6) programName.resize(6);

        vector<pair<unsigned long, string>> machineBytes;
        for (const auto& instruction : instructions) {
            const string formatText = instruction.at("format").get<string>();
            int format = stoi(formatText);
            unsigned long address = hexNumber(instruction.at("locationCounter"), "locationCounter");
            if (format < 1 || format > 4) throw runtime_error("Unsupported instruction format");
            unsigned long opcode = hexNumber(instruction.at("opCode"), "opCode");
            unsigned long code = opcode;
            int byteCount = format;
            if (format == 2) {
                code = opcode << 8; byteCount = 2;
            } else if (format == 3 || format == 4) {
                int n = stoi(instruction.at("n").get<string>()), i = stoi(instruction.at("i").get<string>());
                int x = stoi(instruction.at("x").get<string>()), b = stoi(instruction.at("b").get<string>());
                int p = stoi(instruction.at("p").get<string>()), e = stoi(instruction.at("e").get<string>());
                unsigned long target = hexNumber(instruction.at("targetField"), "targetField");
                if (format == 3 && (e || target > 0xFFF)) throw runtime_error("Invalid format 3 flags or targetField");
                if (format == 4 && (!e || b || p || target > 0xFFFFF)) throw runtime_error("Invalid format 4 flags or targetField");
                int bits = (x << 3) | (b << 2) | (p << 1) | e;
                code = (((opcode & 0xFC) | (n << 1) | i) << (format == 3 ? 16 : 24)) |
                       ((bits << (format == 3 ? 12 : 20)) | target);
                byteCount = format;
            }
            machineBytes.push_back({address, paddedHex(code, byteCount * 2)});
            end = max(end, address + static_cast<unsigned long>(byteCount));
        }

        ofstream objectFile("object_program.txt");
        if (!objectFile) throw runtime_error("Cannot create object_program.txt");
        objectFile << "H" << left << setfill(' ') << setw(6) << programName << right
                   << paddedHex(start, 6) << paddedHex(end - start, 6) << "\n";
        // SIC/XE text records hold at most 30 bytes and cannot span address gaps.
        size_t pos = 0;
        while (pos < machineBytes.size()) {
            unsigned long recordStart = machineBytes[pos].first;
            string record;
            int count = 0;
            unsigned long expected = recordStart;
            while (pos < machineBytes.size() && count < 30 && machineBytes[pos].first == expected) {
                record += machineBytes[pos].second;
                count += static_cast<int>(machineBytes[pos].second.size() / 2);
                expected += static_cast<unsigned long>(machineBytes[pos].second.size() / 2);
                ++pos;
            }
            objectFile << "T" << paddedHex(recordStart, 6) << paddedHex(count, 2) << record << "\n";
        }
        objectFile << "E" << paddedHex(start, 6) << "\n";
        cout << "Wrote object program to object_program.txt" << endl;
    } catch (const exception& error) {
        cerr << "Pass 2 failed: " << error.what() << endl;
        return 1;
    }

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
            cout << "Second byte: " << hex << uppercase << secondByte << endl;

            int thirdByte = targetValue & 0xFF;
            cout << "Third byte: " << hex << uppercase << thirdByte << endl;

            int opcode = (firstByte << 16) | (secondByte << 8) | thirdByte;
            cout << "Opcode: " << hex << uppercase << opcode << endl;
        }
         
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

