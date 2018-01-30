#ifndef COMMON_H
#define COMMON_H

#include "err.h"

#define VERSION "0.0.3-jw"

#define max(a,b) ((a) > (b) ? (a) : (b))

typedef enum {
    // part of the spec
    RESPONSE_ACK,
    RESPONSE_NAK,
    RESPONSE_CANCEL,
    RESPONSE_WAIT,
    // not part of the spec
    RESPONSE_TIMEOUT,
    RESPONSE_NULL
} response_type;

typedef struct _response_t {
    unsigned int packet_num;
    response_type type;
} response_t;

int
convert_channel_num(
    char *s,
    unsigned int *channel_num,
    err_t err
);

int
convert_sample_num(
    char *s,
    unsigned int *channel_num,
    err_t err
);

int
convert_string_to_unsigned_int(
    char *s,
    unsigned int *ui
);

const char *
response_to_string(response_type response);

#endif
