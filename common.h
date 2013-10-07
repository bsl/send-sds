#ifndef __COMMON_H
#define __COMMON_H

#include "err.h"

typedef enum {
    STATE0,  /* seen [],             hoping for f0             */
    STATE1,  /* seen [f0]            hoping for 7e             */
    STATE2,  /* seen [f0,7e]         hoping for channel number */
    STATE3,  /* seen [f0,7e,CN]      hoping for 7{c,d,e,f}     */
    STATE4,  /* seen [f0,7e,CN,x]    hoping for packet number  */
    STATE5,  /* seen [f0,7e,CN,x,PN] hoping for f7             */
} response_state_t;

typedef enum {
    // part of the spec
    RESPONSE_ACK,
    RESPONSE_NAK,
    RESPONSE_CANCEL,
    RESPONSE_WAIT,
    // not part of the spec
    RESPONSE_TIMEOUT,
    RESPONSE_NULL
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

#endif
