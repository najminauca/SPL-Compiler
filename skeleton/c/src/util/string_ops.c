#include "string_ops.h"

#include <util/memory.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

char const *copy(const char *src) {
    char *dst = allocate(strlen(src) + 1);
    strcpy(dst, src);
    return dst;
}

char const *format(const char *fmt, ...) {
    static char buffer[1024];

    va_list ap;

    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);

    return copy(buffer);  // This is a MAJOR memory leak. Because this is only used to make some outputs just before the process is terminated we don't care about it to make our life easier.
}

char const *formatInt(int n, const char *default_value) {
    if (n == INT_MIN) return default_value;
    else return format("%d", n);
}

void repeat(FILE *output, int n, const char *str) {
    if (n > 0) {
        fprintf(output, "%s", str);
        repeat(output, n - 1, str);
    }
}
