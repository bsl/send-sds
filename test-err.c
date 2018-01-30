#include "err.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>

void create_destroy(void);
void set_get(size_t);

void
create_destroy(void)
{
    err_t err = err_create(100);
    assert(err != NULL);
    err_destroy(err);
}

void
set_get(size_t len)
{
    err_t err = err_create(len);
    const char *expected = "expect this string";
    err_set2(err, "%s", expected);
    const char *errstr = err_get(err);
    assert(errstr != NULL);
    assert(strstr(errstr, expected) != NULL);
    err_destroy(err);
}

int
main(void)
{
    create_destroy();
    set_get(1);
    set_get(10);
    set_get(100);
}
