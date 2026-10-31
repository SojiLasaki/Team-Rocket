#include <iostream>
#include <string>
#include <typeinfo>
#include <cctype>
#include <cmath>
#include "helpers.h"
#include <variant>


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
            std::cout << "exp is " << powerResult;
                        std::cout << " operation res " << std::to_string(count) << std::endl;

        }
        else {
            char mult = hexString[i];
            int multInteger = mult - '0';

            

            int powerResult = std::pow(16, exponent);
                        count += (powerResult * multInteger);

            std::cout << "exp is " << powerResult;
                        std::cout << " operation res  " << std::to_string(count) << std::endl;
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


std::string computeTwoHexValues(std::variant<std::string, int> currentLocation, std::variant<std::string,int> increment, ArthmeticOperation operation){
    int currentLocationdDecoded = std::stoi(decodeHexToNumeric(currentLocation));
    int expressionDecoded = std::stoi(decodeHexToNumeric(increment));
    int computedValue;

    switch (operation){

        case ADD:
        computedValue = currentLocationdDecoded + expressionDecoded;

        //Need to then take decodeNumericToHex(computedValue)
        return std::to_string(computedValue);

        case SUB :
        computedValue = currentLocationdDecoded - expressionDecoded;
         //Need to then take decodeNumericToHex(computedValue)
        return std::to_string(computedValue);

        case MULT: 
        computedValue = currentLocationdDecoded * expressionDecoded;
        //Need to then take decodeNumericToHex(computedValue)
        return std::to_string(computedValue);

        default:
        std::cout << "You Silly Rabbit This is an Invalid Operation You Cannot" << currentLocationdDecoded << " " << operation <<  " " << expressionDecoded;
            return "";
    }





}

std::string handleLocationCounterIncr(std::variant<std::string, int> currentLocation, std::variant<std::string,int> increment,  byteBasedOpCode opCodeType) {

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