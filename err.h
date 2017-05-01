#ifndef __ERR_H__
#define __ERR_H__

#include <stddef.h>

/* make err_t opaque */
struct err;
typedef struct err * err_t;

err_t err_create(size_t size);
void err_destroy(err_t err);

void
err_set(
    err_t err,
    const char *,
    const char *,
    const int,
    const char *,
    ...
);

#define err_set2(err, format...) err_set(err, __FILE__, __func__, __LINE__, format)

const char *
err_get(
    err_t err
);

#endif
