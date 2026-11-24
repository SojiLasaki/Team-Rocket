#include "pass1/pass1.h"
#include <iostream>
#include "intermediate/runIntermediate.h"

int main() {
    const std::string sourcePath = "basic.txt";

    if (runPass1(sourcePath) != 0) {
        return 1;
    }

    if (runIntermediate(sourcePath) != 0) {
        return 1;
    }


    return 0;
}
