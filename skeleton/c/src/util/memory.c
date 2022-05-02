/*
 * memory.c -- memory management
 */

#include "memory.h"

#include <wchar.h>
#include <stdlib.h>
#include "errors.h"

void *allocate(unsigned size) {
    void *p;

    p = malloc(size);
    if (p == NULL) {
        error("out of memory");
    }
    return p;
}

void release(void *p) {
    if (p == NULL) {
        error("NULL pointer detected in release");
    }
    free(p);
}
