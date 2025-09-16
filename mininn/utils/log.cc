#include "mininn/utils/log.h"

#include <iostream>
#include <iomanip>

Logger::Logger(LogLevel level, const char* file, int line) : level_(level), file_(file), line_(line) {
    stream_ << "[" << get_current_time() << " " << get_level_name(level) << " " << file_ << ":" << line_ << "] ";
}

Logger::~Logger() {
    std::cout << stream_.str() << std::endl;
}

std::string Logger::get_current_time() {
    std::time_t now = std::time(nullptr);
    std::tm local_time;
    localtime_s(&local_time, &now);
    std::ostringstream oss;
    oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::get_level_name(LogLevel level) {
    switch (level) {
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}