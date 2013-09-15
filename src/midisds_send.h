#ifndef MIDISDS_SEND_H
#define MIDISDS_SEND_H

#include <stdio.h>

#include "midisds_common.h"
#include "midisds_rawmidi.h"

typedef enum {
    STATE0,  /* seen [],             hoping for f0             */
    STATE1,  /* seen [f0]            hoping for 7e             */
    STATE2,  /* seen [f0,7e]         hoping for channel number */
    STATE3,  /* seen [f0,7e,CN]      hoping for 7{c,d,e,f}     */
    STATE4,  /* seen [f0,7e,CN,x]    hoping for packet number  */
    STATE5,  /* seen [f0,7e,CN,x,PN] hoping for f7             */
} midisds_response_state_t;

typedef enum {
    RESPONSE_ACK,
    RESPONSE_NAK,
    RESPONSE_CANCEL,
    RESPONSE_WAIT
} midisds_response_t;

typedef struct {
    char device[10];
    char filename[100];
    unsigned int sysex_channel;
    unsigned int waveform_number;
} midisds_send_file_options_t;

typedef struct {
    char device[10];
    midisds_message_t *msg;
    unsigned int sysex_channel;
    unsigned int waveform_number;
} midisds_send_message_options_t;

ssize_t midisds_send_msg(const midi_t *midi, \
                         midisds_send_message_options_t *options);

ssize_t midisds_send_header(const midi_t *midi, midisds_header_t *hdr);

ssize_t midisds_send_file(midisds_send_file_options_t *options);

void midisds_inspect_send_file_options(midisds_send_file_options_t opts);

#endif
