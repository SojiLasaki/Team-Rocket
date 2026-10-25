#include <iostream>
#include <string>
#include <typeinfo>
#include <cctype>
#include <cmath>
#include "helpers.h"



std::string decodeHexToNumeric(std::string hexString) {
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