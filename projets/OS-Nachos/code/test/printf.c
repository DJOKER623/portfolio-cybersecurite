#include "syscall.h"
#include <stdarg.h>

extern int vsprintf(char *buf, const char *fmt, va_list args);

int printf(const char *fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);
    PutString(buffer);
    return 0;
}
