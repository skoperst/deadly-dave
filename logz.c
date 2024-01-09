#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>

FILE* flog = NULL;

#if defined LINUX || __linux__ || _LINUX
void log_printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
#endif

#if defined WIN32 || __win32__ || _WIN32
void log_printf(const char* fmt, ...)
{
    if (flog == NULL) {
        flog = fopen("dave_log.txt", "w");
    }

    va_list args;
    va_start(args, fmt);

    vfprintf(flog, fmt, args);
    fflush(flog);

    va_end(args);
}
#endif

