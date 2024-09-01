#ifndef LOG_H
#define LOG_H

#include <sstream>
#include <string>

#define LOG(level) Logger(level, __FILE__, __LINE__)

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(LogLevel level, const char* file, int line);

    ~Logger();

    template <typename T>
    Logger& operator<<(const T& msg) {
        stream_ << msg;
        return *this;
    }

    std::string get_current_time();

    std::string get_level_name(LogLevel level);

private:
    LogLevel level_;
    const char* file_;
    int line_;
    std::ostringstream stream_;
};

#endif // LOG_H