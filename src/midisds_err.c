#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "err.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

struct err {
    char *buf;
    size_t buf_size;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

err_t err_create(size_t size) {
    err_t err;

    err = malloc(sizeof(*err));
    err->buf = malloc(size);
    err->buf_size = size;

    err->buf[0] = '\0';

    return err;
}

void err_destroy(err_t err) {
    if (err != NULL) {
        if (err->buf != NULL) {
            free(err->buf);
        }

        free(err);
    }
}

void err_set(err_t err, const char *filename, const char *functionname,
             const int linenum, const char *format, ...) {
    char buf1[256]; /* XXX should not be fixed size */
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf1, sizeof(buf1), format, ap);
    va_end(ap);

    snprintf(err->buf, err->buf_size, "%s, %s(), line %d: %s",
             filename, functionname, linenum, buf1);
}

const char * err_get(err_t err) {
    return (const char *)(err->buf);
}

