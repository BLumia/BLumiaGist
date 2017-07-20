#ifndef __LAJI_LOGGER
#define __LAJI_LOGGER

#include <stdio.h>
#include <pthread.h>

int laji_log_init(const char* path);
int laji_log(const char* buffer);
int laji_log_close();

enum LOG_LEVEL {
    LOG_VERBOSE = 'V',
    LOG_DEBUG = 'D',
    LOG_INFO = 'I',
    LOG_WARN = 'W',
    LOG_ERROR = 'E',
    LOG_ASSERT = 'A'
};

#endif /* __LAJI_LOGGER */