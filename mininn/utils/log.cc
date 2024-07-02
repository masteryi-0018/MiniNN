#include "mininn/utils/log.h"

#include <ctime>
#include <iostream>

void log(const char* file, int line, const char* message) {
    std::time_t now = std::time(nullptr);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::cout << "[" << timeStr << " " << file << ":" << line << "] " << message << std::endl;
}