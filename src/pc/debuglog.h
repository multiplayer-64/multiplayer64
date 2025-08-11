#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include <stdio.h>
#include <time.h>
#include "pc/network/network.h"

static void _debuglog_print_timestamp(void) {
    time_t ltime = time(NULL);
#if defined(_WIN32)
    char* str = asctime(localtime(&ltime));
#else
    struct tm ltime2 = { 0 };
    localtime_r(&ltime, &ltime2);
    char* str = asctime(&ltime2);
#endif
    printf("%.*s", (int)strlen(str) - 1, str);
}

static void _debuglog_print_network_type(void) {
    printf(" [%02d] ", (gNetworkPlayerLocal != NULL) ? gNetworkPlayerLocal->globalIndex : -1);
}

static void _debuglog_print_log_type(char* logType) {
    printf("[%s] ", logType);
}

static void _debuglog_print_short_filename(char* filename) {
    char* last = strrchr(filename, '/');
    if (last != NULL) {
        printf("%s: ", last + 1);
    }
    else {
        printf("???: ");
    }
}

static void _debuglog_print_log(char* logType, char* filename) {
    _debuglog_print_timestamp();
    _debuglog_print_network_type();
    _debuglog_print_log_type(logType);
    _debuglog_print_short_filename(filename);
}

#if defined(DEBUG) && defined(DISABLE_MODULE_LOG)
    #define LOG_DEBUG(...)
    #define LOG_INFO(...)
    #define LOG_ERROR(...) ( _debuglog_print_log("ERROR", __FILE__), printf(__VA_ARGS__), printf("\n") )
#elif defined(DEBUG) && !defined(DISABLE_MODULE_LOG)
    #define LOG_DEBUG(...) ( _debuglog_print_log("DEBUG", __FILE__), printf(__VA_ARGS__), printf("\n") )
    #define LOG_INFO(...)  ( _debuglog_print_log("INFO",  __FILE__), printf(__VA_ARGS__), printf("\n") )
    #define LOG_ERROR(...) ( _debuglog_print_log("ERROR", __FILE__), printf(__VA_ARGS__), printf("\n") )
#else
    #define LOG_DEBUG(...)
    #define LOG_INFO(...)
    #define LOG_ERROR(...)
#endif

#endif