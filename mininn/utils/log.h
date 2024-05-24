#ifndef LOG_H
#define LOG_H

#include <cstring>

#define __FILENAME__ (strrchr(__FILE__, '/') + 1)
#define LOG(message) log(__FILENAME__, __LINE__, message)

void log(const char* file, int line, const char* message);

#endif // LOG_H