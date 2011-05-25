#ifndef _logger_h
#define _logger_h

#include <stdio.h>

#define Logger_error(M, ...) printf("ERROR: [%s:%d] " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define Logger_debug(M, ...) printf("DEGUG: [%s:%d] " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define Logger_trace()       printf("TRACE: [%s:%d] %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__)

#endif
