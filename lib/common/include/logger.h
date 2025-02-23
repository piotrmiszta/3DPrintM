#ifndef LIB_COMMON_LOGGER_H_
#define LIB_COMMON_LOGGER_H_

#include <stdio.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define ORANGE "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define YELLOW "\033[1;33m"
#define RESET "\x1B[0m"

#define _log(stream, type, file, line, ...)                             \
        do {                                                            \
            fprintf(stream, "%s %s:%d -- ", (type), (file), (line));    \
            fprintf(stream, __VA_ARGS__);                               \
            fprintf(stream, "\n");                                      \
        }while(0)

#define LOG_ERROR(...)      \
            _log(stderr, RED"[ERROR]"RESET, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...)    \
            _log(stderr, CYAN"[WARNING]"RESET, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_TRACE(...)      \
            _log(stdout, PURPLE"[TRACE]"RESET, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)       \
            _log(stdout, YELLOW"[INFO]"RESET, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...)      \
            _log(stdout, GREEN"[DEBUG]"RESET, __FILE__, __LINE__, __VA_ARGS__)
#define LOGGER_TRACE


#endif
