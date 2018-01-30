#include "err.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

struct err {
    char *buf;
    size_t buf_size;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

err_t
err_create(size_t size)
{
    err_t err;

    err           = malloc(sizeof(*err));
    err->buf      = malloc(size);
    err->buf_size = size;

    err->buf[0] = '\0';

    return err;
}

void
err_destroy(err_t err)
{
    if (err != NULL) {
        if (err->buf != NULL) {
            free(err->buf);
        }

        free(err);
    }
}

static void
err_realloc(err_t err, size_t new_size)
{
    char *newbuf = (char*) malloc(new_size);
    free(err->buf);
    err->buf = newbuf;
    err->buf_size = new_size;
}

static size_t
va_err_set(
    err_t err,
    const char *filename,
    const char *functionname,
    const int linenum,
    const char *format,
    va_list ap
) {
    int n = snprintf(
        err->buf,
        err->buf_size,
        "%s, %s(), line %d: ",
        filename, functionname, linenum
    );
    if (n < 0)
        abort();

    char *buf;
    size_t size;
    if ((size_t)n < err->buf_size) {
        size = err->buf_size - n;
        buf = err->buf + n;
    }
    else {
        size = 0;
        buf = NULL;
    }
    int n2 = vsnprintf(buf, size, format, ap);
    if (n2 < 0)
        abort();

    return n + n2;
}

void
err_set(
    err_t err,
    const char *filename,
    const char *functionname,
    const int linenum,
    const char *format,
    ...
) {
    va_list ap;
    for (;;) {
        va_start(ap, format);
        size_t n = va_err_set(err, filename, functionname, linenum, format, ap);
        va_end(ap);
        if (n < err->buf_size)
            return;
        err_realloc(err, n + 1);
    }
}

const char *
err_get(err_t err)
{
    return err->buf;
}
