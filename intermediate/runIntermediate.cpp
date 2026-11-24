#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "helpers.h"
#include "../pass2/opcode_table.cpp"
#include "../path_helper.h"

void getUpperCaseString(std::string str)
{
}

std::unordered_map<std::string, std::string> symbolTable;



int runIntermediate(const std::string &sourcePath)
{
    std::string hex;
    std::string res;

    std::string locationCounter;
    std::string startAddress;
        // std::fstream pass_one_file( sourcePath + "_pass1.json");
       const std::string pass1FilePath = inputStem(sourcePath) + "_pass1.json"; 
       std::fstream pass_one_file(pass1FilePath);

    if (pass_one_file.is_open() == false){
        std::cerr << "Error Reading Pass 1 File";
    }

    std::stringstream buffer;
    buffer << pass_one_file.rdbuf();
    std::string pass1Stream = buffer.str();
    pass_one_file.close();


    int contentEnd = pass1Stream.find("]");

    if (contentEnd < 0){
        std::cerr << "Pass 1 File Content is empty";
    }
    std::string fileContentBody =  pass1Stream.substr(1, contentEnd);

    std::vector<std::string> preFormatInstructions;

    int instructionStart = 1;
    int instructionEnd = 0;
    int i = 0;

    fileContentBody.erase(std::remove(fileContentBody.begin(), fileContentBody.end(), ' '), fileContentBody.end());
    fileContentBody.erase(std::remove(fileContentBody.begin(), fileContentBody.end(), '\r'), fileContentBody.end());


    int formatLpEnd = fileContentBody.find("]");

    while (instructionEnd < formatLpEnd  - 1){

        instructionEnd = fileContentBody.find("}",instructionStart);
        int subStrOffset = (instructionEnd - instructionStart) + 1;
        if (instructionEnd > 0){
            preFormatInstructions.push_back(fileContentBody.substr(instructionStart, subStrOffset));
        }
        else {
            break;
        }
        instructionStart = instructionEnd + 2;
    }



    std::vector<std::string> indvInstructionFormatted;
    for (std::string jsonInstruct : preFormatInstructions){
        jsonInstruct.erase(std::remove(jsonInstruct.begin(), jsonInstruct.end(), '{'), jsonInstruct.end());
        jsonInstruct.erase(std::remove(jsonInstruct.begin(), jsonInstruct.end(), '}'), jsonInstruct.end());
        indvInstructionFormatted.push_back(jsonInstruct);
    }


    std::vector<std::string> indvInstructionFormatted2;
    int offset = 0;
    int start = 0;
    int end = 0;
    int lineSize;


    //Create variables that will be assigned read values from pass 1 file
    std::string readSymbol;
    std::string readOpCode;
    std::string readOpCodeHexNumber;
    std::string readOperand;
    std::string operandHexNumber;
    int readN ;
    int readI;
    int readX;
    int readB;
    int readP;
    int readE;

    std::string readLocationCounter;
    std::string  readProgramCounter;
    std::string readTargetFeild;
    std::string readObjectCode;
    int readFormat;
    std::vector<Instruction> readInstructions;
    // std::string readErrorFlag;
    for (std::string formatedJson : indvInstructionFormatted){

        lineSize = formatedJson.size();
        start = 0;
        for (int i = 0; i < lineSize; i++){
            if (formatedJson[i] == '\n'){
                end = i;
                offset = i - start;
                indvInstructionFormatted2.push_back(formatedJson.substr(start, offset));
                start = end + 1;
            }
        }
        int semiColonIndex = 0;
        std::string propertyName;
        std::string propertyValue;
        for (std::string line : indvInstructionFormatted2){
                semiColonIndex = line.find(":");
                std::cout << "Line :" << line << std::endl;
                if (semiColonIndex < 0){
                    std::cerr << "Error Finding SemiColon in Indv Line When reading pass 1 file" << std::endl;
                    std::cout << "Line :" << line << std::endl;
                    std::cout << std::endl;
                    std::cout << std::endl;
                    continue;
                }
                propertyName = line.substr(0,semiColonIndex);

                propertyValue = line.substr(semiColonIndex + 1);
                propertyName.erase(std::remove(propertyName.begin(), propertyName.end(), '"'), propertyName.end());
                propertyValue.erase(std::remove(propertyValue.begin(), propertyValue.end(), '"'), propertyValue.end());

                //Remove trailing , for string type values
                if (propertyName == "symbol") {
                    readSymbol = propertyValue.substr(0, propertyValue.size() - 1);
                } else if (propertyName == "opCode") {
                    readOpCode = propertyValue.substr(0, propertyValue.size() - 1);;
                } else if (propertyName == "opCodeHexNumber") {
                    readOpCodeHexNumber = propertyValue.substr(0, propertyValue.size() - 1);;
                } else if (propertyName == "operand") {
                    readOperand = propertyValue.substr(0, propertyValue.size() - 1);;
                } else if (propertyName == "operandHexNumber") {
                    operandHexNumber = propertyValue.substr(0, propertyValue.size() - 1);;
                } else if (propertyName == "n") {
                    readN = std::stoi(propertyValue);
                } else if (propertyName == "i") {
                    readI = std::stoi(propertyValue);
                } else if (propertyName == "x") {
                    readX = std::stoi(propertyValue);
                } else if (propertyName == "b") {
                    readB = std::stoi(propertyValue);
                } else if (propertyName == "p") {
                    readP = std::stoi(propertyValue);
                } else if (propertyName == "e") {
                    readE = std::stoi(propertyValue);
                } else if (propertyName == "locationCounter") {
                    readLocationCounter = propertyValue.substr(0, propertyValue.size() - 1);;
                } else if (propertyName == "programCounter") {
                    readProgramCounter = propertyValue.substr(0, propertyValue.size() - 1);;
                } else if (propertyName == "targetFeild") {
                    readTargetFeild = propertyValue.substr(0, propertyValue.size() - 1);;
                } else if (propertyName == "objectCode") {
                    readObjectCode = propertyValue.substr(0, propertyValue.size() - 1);;
                } else if (propertyName == "format") {
                    readFormat = std::stoi(propertyValue);
                }    
        }

        Instruction readInstruction =  Instruction{
                    readSymbol,
                    readOpCode,
                    readOpCodeHexNumber,
                    readOperand,
                    operandHexNumber,
                    readN,
                    readI,
                    readX,
                    readB,
                    readP,
                    readE,
                    readLocationCounter,
                    readProgramCounter,
                    readTargetFeild,
                    readObjectCode,
                    readFormat
                };

        readInstructions.push_back(readInstruction);
        
    }


    int count3 = 0;
    for (Instruction item : readInstructions)
    {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Item Count " << count3 << std::endl;
        std::cout << "symbol: " << item.symbol << std::endl;
        std::cout << "opCode: " << item.opCode << std::endl;
        std::cout << "opCodeHexNumber: " << item.opCodeHexNumber << std::endl;
        std::cout << "operand: " << item.operand << std::endl;
        std::cout << "operandHexNumber: " << item.operandHexNumber << std::endl;
        std::cout << "n: " << item.n << std::endl;
        std::cout << "i: " << item.i << std::endl;
        std::cout << "x: " << item.x << std::endl;
        std::cout << "b: " << item.b << std::endl;
        std::cout << "p: " << item.p << std::endl;
        std::cout << "e: " << item.e << std::endl;
        std::cout << "locationCounter: " << item.locationCounter << std::endl;
        std::cout << "programCounter: " << item.programCounter << std::endl;
        std::cout << "targetFeild: " << item.targetFeild << std::endl;
        std::cout << "objectCode: " << item.objectCode << std::endl;
        std::cout << "format: " << item.format << std::endl;
        std::cout << "ErrorFlag: " << item.errorFlag << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        ++count3;

        std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }


    std::vector<Instruction> outPutItems;
    int vecEnd = readInstructions.size();

    for (int i = 0; i < vecEnd; i++)
    {


        if (i == 0){ 
        Instruction firstItem = readInstructions[0];
        std::string firstItemSym = readInstructions[0].opCode;
        std::transform(firstItemSym.begin(), firstItemSym.end(), firstItemSym.begin(), [](unsigned char c)
                    { return std::toupper(c); });
        if (firstItemSym == "START")
        {
            //Will need to decode this numeric to hex before storing globals
            startAddress = firstItem.operand;
            locationCounter = firstItem.operand;

        }
        else
        {
            locationCounter = "0";
            startAddress = "0";
        }
        }

        // 1.)Write Symbol and locctr to table
        if (readInstructions[i].symbol != ""){

            if (isSymbolUnique(readInstructions[i].symbol))
            {
                addSymbol(readInstructions[i].symbol, locationCounter);
                readInstructions[i].locationCounter = locationCounter;
            }
            else
            {
                readInstructions[i].errorFlag = "Error Duplicated Symbol "  + readInstructions[i].symbol;
                continue;
            }
        }

        // 2.) Detetct OpCode
        if (readInstructions[i].opCode != "")
            {
                auto opCodeFormat = opcodeTable.find(readInstructions[i].opCode);

                std::string itemOpCode = readInstructions[i].opCode;
                std::transform(itemOpCode.begin(), itemOpCode.end(), itemOpCode.begin(), [](unsigned char c)
                { return std::toupper(c); });

                ByteBasedOpCode opCodeToEnum = itemOpCode == "RESW" ? ByteBasedOpCode::RESW : itemOpCode == "RESB" ? ByteBasedOpCode::RESB : ByteBasedOpCode::NEITHER;

                if (opCodeFormat != opcodeTable.end())
                {
                    readInstructions[i].locationCounter = locationCounter;
                    readInstructions[i].opCodeHexNumber = decodeNumericToHex(opCodeFormat->second.opcode);
                    readInstructions[i].format = opCodeFormat->second.format;
                    locationCounter = handleLocationCounterIncr(locationCounter, 3, opCodeToEnum);
                } 
            }
        // outPutItems.push_back(formattedInstruction);

        

    }


    //3.) Set Program Counter
    for (int i = 0; i < vecEnd; i++){
        Instruction nextItem = readInstructions[i + 1];
        readInstructions[i].programCounter = nextItem.locationCounter;
        //at the end last item program counter is end of program 
        if (i == vecEnd - 1){
            readInstructions[i].programCounter = locationCounter;
        }
    }

std::cout << "\n==================== SYMBOL TABLE ====================\n";
for (const auto& [symbol, address] : symbolTable) {
    std::cout << "  " << symbol << "  :  " << address << "\n";
}
std::cout << "======================================================\n\n";
    //4.) Calculate x,p,b,e and target feild
    for (int i = 0; i < vecEnd; i++) {
        ComputeAndUpdateInstructionAddressModes(readInstructions[i]);
    }


    int count = 0;
    for (Instruction item : readInstructions)
    {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Item Count " << count << std::endl;
        std::cout << "symbol: " << item.symbol << std::endl;
        std::cout << "opCode: " << item.opCode << std::endl;
        std::cout << "opCodeHexNumber: " << item.opCodeHexNumber << std::endl;
        std::cout << "operand: " << item.operand << std::endl;
        std::cout << "operandHexNumber: " << item.operandHexNumber << std::endl;
        std::cout << "n: " << item.n << std::endl;
        std::cout << "i: " << item.i << std::endl;
        std::cout << "x: " << item.x << std::endl;
        std::cout << "b: " << item.b << std::endl;
        std::cout << "p: " << item.p << std::endl;
        std::cout << "e: " << item.e << std::endl;
        std::cout << "locationCounter: " << item.locationCounter << std::endl;
        std::cout << "programCounter: " << item.programCounter << std::endl;
        std::cout << "targetFeild: " << item.targetFeild << std::endl;
        std::cout << "objectCode: " << item.objectCode << std::endl;
        std::cout << "format: " << item.format << std::endl;
        std::cout << "ErrorFlag: " << item.errorFlag << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        ++count;
    }
    
    WriteInstructionsToOutPutFile(inputStem(sourcePath) + "_intermediate.json", readInstructions);
    return 0;
}
