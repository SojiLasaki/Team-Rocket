#include <iostream>
#include <string>

// Converts a binary string (e.g. "11010110") to uppercase hexadecimal.
// Returns an empty string if the input contains non-binary characters.
std::string binToHex(const std::string &bin) {
    if (bin.empty()) return "";
    for (char c : bin)
        if (c != '0' && c != '1') return "";

    // Pad on the left so the length is a multiple of 4
    std::string padded = std::string((4 - bin.size() % 4) % 4, '0') + bin;

    const char *digits = "0123456789ABCDEF";
    std::string hex;
    for (size_t i = 0; i < padded.size(); i += 4) {
        int v = (padded[i] - '0') * 8 + (padded[i + 1] - '0') * 4 +
                (padded[i + 2] - '0') * 2 + (padded[i + 3] - '0');
        hex += digits[v];
    }
    return hex;
}

int main() {
    std::string bin;
    std::cout << "Enter binary: ";
    std::cin >> bin;

    std::string hex = binToHex(bin);
    if (hex.empty()) {
        std::cerr << "Invalid binary input\n";
        return 1;
    }
    std::cout << "Hex: " << hex << "\n";
    return 0;
}
