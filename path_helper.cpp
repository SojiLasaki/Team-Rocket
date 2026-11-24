#include "path_helper.h"
#include <string>
#include <filesystem>

std::string inputStem(const std::string& inputPath) {
    return std::filesystem::path(inputPath)
        .replace_extension()
        .string();
}