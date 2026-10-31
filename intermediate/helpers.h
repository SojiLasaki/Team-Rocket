
#include <iostream>

#include <variant>

using conditionalType = std::variant<std::string, int>;
std::string decodeHexToNumeric(conditionalType hexValue);
int decodeLetterHexToNumeric( char letter);

enum ArthmeticOperation {
    MULT,
    ADD ,
    SUB,
};
std::string computeTwoHexValues(conditionalType currentLocation, conditionalType increment, ArthmeticOperation operation);

enum byteBasedOpCode {
    RESB,
    RESW,
    NEITHER,
};
std::string handleLocationCounterIncr(conditionalType currentLocation, conditionalType increment, byteBasedOpCode opCodeType );