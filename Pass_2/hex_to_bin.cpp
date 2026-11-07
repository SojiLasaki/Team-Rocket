#include <iostream>
#include <string>

// Converts a hexadecimal string (optional "0x" prefix) to binary.
// Returns an empty string if the input contains non-hex characters.
std::string hexToBin(std::string hex) {
    if (hex.size() >= 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))
        hex = hex.substr(2);
    if (hex.empty()) return "";

    std::string bin;
    for (char c : hex) {
        int v;
        if (c >= '0' && c <= '9') v = c - '0';
        else if (c >= 'a' && c <= 'f') v = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') v = c - 'A' + 10;
        else return "";

        for (int b = 3; b >= 0; b--) bin += ((v >> b) & 1) ? '1' : '0';
    }
    return bin;
}

int main() {
    std::string hex;
    std::cout << "Enter hex: ";
    std::cin >> hex;

    std::string bin = hexToBin(hex);
    if (bin.empty()) {
        std::cerr << "Invalid hex input\n";
        return 1;
    }
    std::cout << "Binary: " << bin << "\n";
    return 0;
}
