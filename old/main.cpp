#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <cctype>
#include <cmath>

#include "./intermediate/helpers.h"

void getUpperCaseString (std::string str) {


}

int main(){


std::string hex;
std::string res;

int locationCounter;
int startAddress;
// std::cout << "Enter String :";

// std::cin >> hex;
res = decodeHexToNumeric("7FFFFFFF");
std::cout << "result is" << res;

//Pass 2 Start 
struct Items {
        public: 
        std::string symbol;
        std::string opCode;
        std::string operand;
        int n;
        int i;
        int x;
        int b;
        int p;
        int e;
        int locationCounter;
        int programCounter;
        int targetFeild;
        int objectCode;
        int format;
        
        

};

Items item1 =  Items{"start", "LDA", "0000"};
Items item2 =  Items{"first", "LDA", "#05"};
Items item3 =  Items{"", "STL", "#33"};
Items item4 =  Items{"second", "LDB", "300000"};
Items item5 =  Items{"", "LDX", "#40"};
std::vector<Items> vecItems;

vecItems.push_back(item1);
vecItems.push_back(item2);
vecItems.push_back(item3);
vecItems.push_back(item4);
vecItems.push_back(item5);


//find start

// Items firstItem = vecItems[0];
// std::string firstItemSym = vecItems[0].symbol;


// //Turn to upper case
// std::transform(firstItemSym.begin(), firstItemSym.end(), firstItemSym.begin(),[](unsigned char c) {
//     return std::toupper(c);
//   });
// // getUpperCaseString(firstItemSym);

// std::cout << "first item should be upper case" << firstItemSym;

// if ( firstItemSym == "START"){
//     startAddress = std::stoi(firstItem.operand);
//     locationCounter = std::stoi(firstItem.operand);
//     std::cout << "start address: "  << startAddress << "Location Counter : " << locationCounter;
//     continue;
// }
// else {
//     locationCounter = 0;
//     startAddress = 0;
//     continuel
// }


for (Items item : vecItems){
    Items firstItem = vecItems[0];
    std::string firstItemSym = vecItems[0].symbol;

    //Turn to upper case
    std::transform(firstItemSym.begin(), firstItemSym.end(), firstItemSym.begin(),[](unsigned char c) {
        return std::toupper(c);
    });

    std::cout << "first item should be upper case" << firstItemSym;

    if ( firstItemSym == "START"){
    startAddress = std::stoi(firstItem.operand);
    locationCounter = std::stoi(firstItem.operand);
    std::cout << "start address: "  << startAddress << "Location Counter : " << locationCounter;
    continue;
    }
    else {
        locationCounter = 0;
        startAddress = 0;
        continue;
    }
}

    return 0;
}