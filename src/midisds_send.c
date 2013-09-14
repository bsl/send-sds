#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sndfile.h>

#include "midisds_common.h"
#include "midisds_err.h"
#include "midisds_rawmidi.h"
#include "midisds_send.h"

// Send an sds message to a device.
int midisds_send_msg(const midi_t *midi, const midisds_message *msg) {
    midisds_header *hdr = msg->hdr;
    return midisds_send_header(midi, hdr);
}

int midisds_send_header(const midi_t *midi, midisds_header *hdr) {
    size_t hdr_len = midisds_header_length();
    return midisds_send(midi, *hdr, hdr_len);
}
