
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

enum ByteBasedOpCode {
    RESB,
    RESW,
    NEITHER,
};
std::string handleLocationCounterIncr(conditionalType currentLocation, conditionalType increment, ByteBasedOpCode opCodeType );

    struct Instruction
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

    std::string locationCounter;
    std::string  programCounter;
    std::string targetFeild;
    std::string objectCode;
    int format = 0;
    std::string errorFlag;
    };

    enum Pass1LinePropertyName {
                    symbol,
                    opCode,
                    opCodeHexNumber,
                    operand,
                    operandHexNumber,
                    n,
                    i,
                    x,
                    b,
                    p,
                    e,
                    locationCounter,
                    programCounter,
                    targetFeild,
                    objectCode,
                    format
                };
void ComputeAndUpdateInstructionAddressModes(Instruction& item);


extern std::unordered_map<std::string, std::string> symbolTable;

void addSymbol(std::string symbolName, std::string locationCounter);
bool isSymbolUnique(std::string symbolName);
void computeInstructionXBPEandTA(Instruction& instruction, std::string symbol);
std::string decodeNumericToHex(int numericValue);
void readPassOneFile(std::string fileName);