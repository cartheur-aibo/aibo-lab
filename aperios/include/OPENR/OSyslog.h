#ifndef APERIOS_HOST_OPENR_OSYSLOG_H
#define APERIOS_HOST_OPENR_OSYSLOG_H

#include <cstdarg>
#include <cstdio>

enum {
    osyslogERROR = 3,
};

inline void aperios_host_vprint(FILE* stream, const char* prefix, const char* fmt, va_list ap) {
    std::fputs(prefix, stream);
    std::vfprintf(stream, fmt, ap);
}

inline void aperios_host_debug(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    aperios_host_vprint(stderr, "[OSYSDEBUG] ", fmt, ap);
    va_end(ap);
}

inline void aperios_host_print(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::vprintf(fmt, ap);
    va_end(ap);
}

inline void aperios_host_log1(int level, const char* fmt, ...) {
    (void)level;
    va_list ap;
    va_start(ap, fmt);
    aperios_host_vprint(stderr, "[OSYSLOG1] ", fmt, ap);
    std::fputc('\n', stderr);
    va_end(ap);
}

#define OSYSDEBUG(args) do { aperios_host_debug args; } while (0)
#define OSYSPRINT(args) do { aperios_host_print args; } while (0)
#define OSYSLOG1(args) do { aperios_host_log1 args; } while (0)

#endif
