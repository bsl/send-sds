#ifndef MIDISD_SEND_H
#define MIDISD_SEND_H

#include <stdio.h>

#include "midisd_common.h"
#include "midisd_rawmidi.h"

typedef enum {
    STATE0,  /* seen [],             hoping for f0             */
    STATE1,  /* seen [f0]            hoping for 7e             */
    STATE2,  /* seen [f0,7e]         hoping for channel number */
    STATE3,  /* seen [f0,7e,CN]      hoping for 7{c,d,e,f}     */
    STATE4,  /* seen [f0,7e,CN,x]    hoping for packet number  */
    STATE5,  /* seen [f0,7e,CN,x,PN] hoping for f7             */
} midisd_response_state_t;

typedef enum {
    RESPONSE_ACK,
    RESPONSE_NAK,
    RESPONSE_CANCEL,
    RESPONSE_WAIT
} midisd_response_t;

typedef struct {
    char device[10];
    char filename[100];
    unsigned int sysex_channel;
    unsigned int waveform_number;
} midisd_send_file_options_t;

typedef struct {
    midisd_message_t *msg;
    unsigned int sysex_channel;
    unsigned int waveform_number;
} midisd_send_message_options_t;

ssize_t midisd_send_msg(const midi_t *midi,
                         midisd_send_message_options_t *options);

ssize_t midisd_send_header(const midi_t *midi, midisd_header_t *hdr);

ssize_t midisd_send_file(midisd_send_file_options_t *options);

void midisd_inspect_send_file_options(midisd_send_file_options_t opts);

#endif
