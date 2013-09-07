#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "err.h"
#include "midisds_rawmidi.h"
#include "midisds.h"

typedef enum {
    STATE0,  /* seen [],             hoping for f0             */
    STATE1,  /* seen [f0]            hoping for 7e             */
    STATE2,  /* seen [f0,7e]         hoping for channel number */
    STATE3,  /* seen [f0,7e,CN]      hoping for 7{c,d,e,f}     */
    STATE4,  /* seen [f0,7e,CN,x]    hoping for packet number  */
    STATE5,  /* seen [f0,7e,CN,x,PN] hoping for f7             */
} response_state_t;

typedef enum {
    RESPONSE_ACK,
    RESPONSE_NAK,
    RESPONSE_CANCEL,
    RESPONSE_WAIT
} response_t;

int midisds_send(const char *filename, int *fd);

