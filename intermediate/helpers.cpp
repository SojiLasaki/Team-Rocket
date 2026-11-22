#include <iostream>
#include <string>
#include <typeinfo>
#include <cctype>
#include <cmath>
#include "helpers.h"
#include <variant>
#include <sstream>
#include <algorithm>
#include <fstream>

std::string decodeHexToNumeric(std::variant<std::string, int> hexValue) {

    std::string hexString;
    if (std::holds_alternative<int>(hexValue)) {
    hexString = std::to_string(std::get<int>(hexValue));
    } else {
        hexString = std::get<std::string>(hexValue);
    }

    std::cout << "Hex String " << hexString << std::endl;
    if (hexString == "") {
        return "";
    }

    int end = hexString.size() - 1;
    int count = 0;
    for (int i = 0; i <= end; i++) {

        int exponent = end == 0 ? 0 : end - i;
        if (std::isalpha(hexString[i])) {
            int mult = decodeLetterHexToNumeric(hexString[i]);

            int powerResult = std::pow(16, exponent);

            count += (powerResult * mult);
            // std::cout << "exp is " << powerResult;
            //             std::cout << " operation res " << std::to_string(count) << std::endl;

        }
        else {
            char mult = hexString[i];
            int multInteger = mult - '0';

            

            int powerResult = std::pow(16, exponent);
                        count += (powerResult * multInteger);

            // std::cout << "exp is " << powerResult;
            //             std::cout << " operation res  " << std::to_string(count) << std::endl;
        }

    }

    return std::to_string(count);
}

int decodeLetterHexToNumeric( char letter) {

    if (std::to_string(letter) == "") {
        return 0;
    }

    char letterUpper = std::toupper(letter);
    switch (letterUpper) {
        case 'A': 
        return 10;

        case 'B':
        return 11;

        case 'C':
        return 12;

        case 'D':
        return 13;

        case 'E':
        return 14;
        
        case 'F':
        return 15;

        default:
        return 0;
    }

    return 0;
}


std::string decodeNumericToHex(int numericValue){

    std::string computedStr;
    std::stringstream ss;
    ss << std::hex << numericValue;
    computedStr = ss.str();

    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
    std::cout << "Hex String " << computedStr << std::endl;
    return computedStr;
}
std::string computeTwoHexValues(std::variant<std::string, int> currentLocation, std::variant<std::string,int> increment, ArthmeticOperation operation){
    int currentLocationdDecoded = std::stoi(decodeHexToNumeric(currentLocation));
    int expressionDecoded = std::stoi(decodeHexToNumeric(increment));
    int computedValue;
    std::string computedStr;
    std::stringstream ss;

    switch (operation){

        case ADD:
        computedValue = currentLocationdDecoded + expressionDecoded;

        ss << std::hex << computedValue;
        computedStr = ss.str();
        return computedStr;

        case SUB :
        computedValue = currentLocationdDecoded - expressionDecoded;
        ss << std::hex << computedValue;
        computedStr = ss.str();
        return computedStr;

        case MULT: 
        computedValue = currentLocationdDecoded * expressionDecoded;
        ss << std::hex << computedValue;
        computedStr = ss.str();
        return computedStr;

        default:
        std::cout << "You Silly Rabbit This is an Invalid Operation You Cannot" << currentLocationdDecoded << " " << operation <<  " " << expressionDecoded;
            return "";
    }





}

std::string handleLocationCounterIncr(std::variant<std::string, int> currentLocation, std::variant<std::string,int> increment,  ByteBasedOpCode opCodeType) {

    int currentLocationdDecoded = std::stoi(decodeHexToNumeric(currentLocation));
    int expressionDecoded = std::stoi(decodeHexToNumeric(increment));
    std::string result;
    switch (opCodeType) {
        case RESB:
        result = computeTwoHexValues(currentLocation,increment, ADD);
        return result;

        case RESW:
        result = computeTwoHexValues(currentLocation,increment, MULT);
        return result;

        case NEITHER:
        result = computeTwoHexValues(currentLocation,increment, ADD);
        return result;

        default :
        std::cout << "Are you mad? You tried Computing a location Counter with Invalid Terms" 
        << currentLocationdDecoded << " " << opCodeType << " " << expressionDecoded;


    }
}

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

void ComputeAndUpdateInstructionAddressModes(Instruction& item) {
        std::string symbol;

        //Case where format 2 instructions and 2 operands EX: ADDR S,X
        if (!item.operand.empty() && item.format == 2 && item.operand.find(",") != std::string::npos){
            int firstOperandIndex = item.operand.find(",");
            std::string firstOperand = item.operand.substr(0, firstOperandIndex);
            std::string secondOperand = item.operand.substr(firstOperandIndex + 1);

            std::cout << "First Operand " << firstOperand << " Second operand " << secondOperand << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;

            //Insert First operand register value
            std::transform(firstOperand.begin(), firstOperand.end(), firstOperand.begin(), [](unsigned char c)
            { return std::toupper(c); });

            if (firstOperand == "A") {
                item.targetFeild = "0";
            } else if (firstOperand == "X") {
                item.targetFeild = "1";
            } else if (firstOperand == "L") {
                item.targetFeild = "2";
            } else if (firstOperand == "B") {
                item.targetFeild = "3";
            } else if (firstOperand == "S") {
                item.targetFeild = "4";
            } else if (firstOperand == "T") {
                item.targetFeild = "5";
            } else if (firstOperand == "F") {
                item.targetFeild = "6";
            } else if (firstOperand == "PC") {
                item.targetFeild = "8";
            } else if (firstOperand == "SW") {
                item.targetFeild = "9";
            }

            //Insert Second operand register value
            std::transform(secondOperand.begin(), secondOperand.end(), secondOperand.begin(), [](unsigned char c)
            { return std::toupper(c); });
            if (secondOperand == "A") {
                item.targetFeild += "0";
            } else if (secondOperand == "X") {
                item.targetFeild += "1";
            } else if (secondOperand == "L") {
                item.targetFeild += "2";
            } else if (secondOperand == "B") {
                item.targetFeild += "3";
            } else if (secondOperand == "S") {
                item.targetFeild += "4";
            } else if (secondOperand == "T") {
                item.targetFeild += "5";
            } else if (secondOperand == "F") {
                item.targetFeild += "6";
            } else if (secondOperand == "PC") {
                item.targetFeild += "8";
            } else if (secondOperand == "SW") {
                item.targetFeild += "9";
            }

            return;
        }



        //Case where format 2 instructions and 1 operand EX: TIXR T
        if (!item.operand.empty() && item.format == 2){    
            std::string operand = item.operand;
                std::transform(operand.begin(), operand.end(), operand.begin(), [](unsigned char c)
                { return std::toupper(c); });

                if (operand == "A") {
                    item.targetFeild += "00";
                } else if (operand == "X") {
                    item.targetFeild += "10";
                } else if (operand == "L") {
                    item.targetFeild += "20";
                } else if (operand == "B") {
                    item.targetFeild += "30";
                } else if (operand == "S") {
                    item.targetFeild += "40";
                } else if (operand == "T") {
                    item.targetFeild += "50";
                } else if (operand == "F") {
                    item.targetFeild += "60";
                } else if (operand == "PC") {
                    item.targetFeild += "80";
                } else if (operand == "SW") {
                    item.targetFeild += "90";
                }
                return;

        }
        if (!item.operand.empty() && item.operand.find("#") != std::string::npos && item.operand.find("#") == 0){
            //Will have to handle cases were we are passsed #maxlen
            //Currenyly only handle #300 cases
            item.n = 0;
            item.i = 1;
            int operand;
            std::stringstream ss;
            std::string computedvalue;
            try {
                operand = stoi(item.operand.substr(1));
                ss << std::hex << operand;
                computedvalue = ss.str();
                
            }
            catch (const std::invalid_argument&){
                
                item.errorFlag = "Invalid Operand Value" + item.operand;
                return;
            }
            
            item.targetFeild = computedvalue;
            return;

        }

        else if (!item.operand.empty() && item.operand.find(",") != std::string::npos){
            item.n = 1;
            item.i = 1;
            item.x = 1;
            symbol = item.operand.substr(0, item.operand.find(","));
            computeInstructionXBPEandTA(item, symbol);
            // std::cout << "I found an indexed symbol " << symbol << std::endl;
            // auto it = symbolTable.find(symbol);
            // if (it != symbolTable.end()) {
            //     auto it = symbolTable.find(symbol);
            //     symbolLocation = it->second;
            //     dispHex = computeTwoHexValues(symbolLocation, item.programCounter,SUB);
            //     dispNumeric = stoi(decodeHexToNumeric(dispHex));
            //     std::cout << "Distance Hex " << dispHex << std::endl;
            //     std::cout << "Distance Numeric " << dispNumeric << std::endl;
                
            //     if (dispNumeric <= 2047){
            //         item.p = 1;
            //         item.targetFeild = dispHex;
            //     }

            //     else if (dispNumeric <= 4095){
            //         item.b = 1;
            //         item.targetFeild = dispHex;
            //     }

            //     else {
            //         item.e = 1;
            //         item.targetFeild = dispHex;
            //     }
            // }
        
            // else {
            //     item.errorFlag = "Referenced a unidentified symbol for adressing.";
            // }
            // std::cout << "I have a indexed adrress at " << item.operand << std::endl;
        }
        
        else if (!item.operand.empty() && item.operand.find("@") != std::string::npos && item.operand.find("@") == 0){
            item.n = 1;
            item.i = 0;
            symbol = item.operand.substr(item.operand.find("@") + 1);

            std:: cout << "@ Address found symbol " << symbol << "index = " << item.operand.find("@") << std::endl;
            computeInstructionXBPEandTA(item, symbol);

        //     if (it != symbolTable.end()) {
        //         symbolLocation = it->second;
        //         dispHex = computeTwoHexValues(symbolLocation, item.programCounter,SUB);
        //         dispNumeric = stoi(decodeHexToNumeric(dispHex));
        //         std::cout << "Distance Hex " << dispHex << std::endl;
        //         std::cout << "Distance Numeric " << dispNumeric << std::endl;
                
        //         if (dispNumeric <= 2047){
        //             item.p = 1;
        //             item.targetFeild = dispHex;
        //         }

        //         else if (dispNumeric <= 4095){
        //             item.b = 1;
        //             item.targetFeild = dispHex;
        //         }

        //         else {
        //             item.e = 1;
        //             item.targetFeild = dispHex;
        //         }
        //         return;
        // }
        }

        //Handle indirect addressing of operand case
        else if (typeid(item.operand) == typeid(int)){
            item.n = 1;
            item.i = 1;
            computeInstructionXBPEandTA(item, item.symbol);
        }

        //handle 
        else {
            item.n = 1;
            item.i = 1;
            std::cout << "I found an indirect symbol " << symbol << std::endl;
            computeInstructionXBPEandTA(item, item.operand);
        //     if (it != symbolTable.end()) {
        //         symbolLocation = it->second;
        //         dispHex = computeTwoHexValues(symbolLocation, item.programCounter,SUB);
        //         dispNumeric = stoi(decodeHexToNumeric(dispHex));
        //         std::cout << "Distance Hex " << dispHex << std::endl;
        //         std::cout << "Distance Numeric " << dispNumeric << std::endl;
                
        //         if (dispNumeric <= 2047){
        //             item.p = 1;
        //             item.targetFeild = dispHex;
        //         }

        //         else if (dispNumeric <= 4095){
        //             item.b = 1;
        //             item.targetFeild = dispHex;
        //         }

        //         else {
        //             item.e = 1;
        //             item.targetFeild = dispHex;
        //         }
        //         return;
        // }
    }

    return;
        }


void computeInstructionXBPEandTA(Instruction& item, std::string symbol){
            std::string dispHex;
            int dispNumeric;
            std::string symbolLocation;
            symbol.erase(std::remove(symbol.begin(), symbol.end(), ' '), symbol.end());

            std::cout << "Symbol " << symbol << std::endl;
            std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            auto it = symbolTable.find(symbol);
            if (it != symbolTable.end()) {
                auto it = symbolTable.find(symbol);
                symbolLocation = it->second;
                dispHex = computeTwoHexValues(symbolLocation, item.programCounter,SUB);
                dispNumeric = stoi(decodeHexToNumeric(dispHex));
                std::cout << "Distance Hex " << dispHex << std::endl;
                std::cout << "Distance Numeric " << dispNumeric << std::endl;
                
                if (dispNumeric <= 2047){
                    item.p = 1;
                    item.targetFeild = dispHex;
                }

                else if (dispNumeric <= 4095){
                    item.b = 1;
                    item.targetFeild = dispHex;
                }

                else {
                    item.e = 1;
                    item.targetFeild = dispHex;
                }

            }
            else {
                item.errorFlag = "Referenced a unidentified symbol for adressing.";
                }
            return;
}


void WriteInstructionsToOutPutFile(std::string outputFileName, std::vector<Instruction> instructions){
    std::ofstream outputFile(outputFileName, std::ios::app);
    if (!outputFile.is_open()){
        std::cerr << "Error Could Not Write to Output File Named: " << outputFileName << std::endl;
        return;
    }

    int end = instructions.size();
    outputFile << "[";
    for (int i = 0; i < end; i++){
        std::string instructEnd = i == (end - 1) ?  "}\n" : "},";
        outputFile <<  "\n\t" <<"{\n\t\t\"symbol\" : \"" << instructions[i].symbol << "\",\n"
        << "\t\t\"opCode\" : \"" << instructions[i].opCode << "\", \n"
        << "\t\t\"opCodeHexNumber\" : \"" << instructions[i].opCodeHexNumber << "\", \n"
        << "\t\t\"operand\" : \"" << instructions[i].operand << "\", " << "\n"
        << "\t\t\"operandHexNumber\" : \"" << instructions[i].operandHexNumber << "\", \n"
        << "\t\t\"n\" : \"" << instructions[i].n << "\", \n"
        << "\t\t\"i\" : \"" << instructions[i].i << "\", \n"
        << "\t\t\"x\" : \"" << instructions[i].x << "\", \n"
        << "\t\t\"b\" : \"" << instructions[i].b << "\", \n"
        << "\t\t\"p\" : \"" << instructions[i].p << "\", \n"
        << "\t\t\"e\" : \"" << instructions[i].e << "\", \n" 
        << "\t\t\"locationCounter\" : \"" << instructions[i].locationCounter << "\", " << "\n"
        << "\t\t\"programCounter\" : \"" << instructions[i].programCounter << "\", " << "\n"
        << "\t\t\"targetFeild\" : \"" << instructions[i].targetFeild << "\", " << "\n"
        << "\t\t\"objectCode\" : \"" << instructions[i].objectCode << "\", " << "\n"
        << "\t\t\"format\" : \"" << instructions[i].format << "\", "<< "\n"
        << "\t\t\"ErrorFlag\" : \"" << instructions[i].errorFlag << "\"\n\t" << instructEnd;
    }
    outputFile << "]";

    outputFile.close();
}

