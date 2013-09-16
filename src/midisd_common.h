#ifndef MIDISD_COMMON_H
#define MIDISD_COMMON_H

#include <stdio.h>

#include "midisd_log.h"

// ====
// defs
// ====

#define MIDISD_HEADER_LENGTH                  21
#define MIDISD_PACKET_LENGTH                  127
#define MIDISD_AUDIO_BYTES_PER_PACKET         120

// jacked from libsndfile/src/sds.c
#define MIDI_SDS_3BYTE_TO_INT_DECODE(x) \
    (((x) & 0x7F) | (((x) & 0x7F00) >> 1) | (((x) & 0x7F0000) >> 2))
#define MIDI_SDS_INT_TO_3BYTE_ENCODE(x) \
    (((x) & 0x7F) | (((x) << 1) & 0x7F00) | (((x) << 2) & 0x7F0000))

#define MIDI_SDS_MAX(a,b) ((a) > (b) ? (a) : (b))

// =====
// types
// =====

// midisd_byte
typedef unsigned char midisd_byte_t;

// midisd_header
typedef midisd_byte_t midisd_header_t[MIDISD_HEADER_LENGTH];

// midisd_packet
typedef midisd_byte_t midisd_packet_t[MIDISD_PACKET_LENGTH];

// midisd_message
typedef struct {
    midisd_header_t hdr;
    midisd_packet_t *pkts;
    ssize_t num_packets;
} midisd_message_t;

// =========
// functions
// =========

/* midisd_message midisd_read_file(const char *filename, int *fd_r); */

ssize_t midisd_header_length(void);
ssize_t midisd_packet_length(void);
ssize_t midisd_audio_bytes_per_packet(void);

// midisd_header_t *midisd_get_header(midisd_send_message_options_t *opts);
midisd_header_t *midisd_get_header(midisd_message_t *msg);

void midisd_copy_header(midisd_header_t dest, midisd_header_t src);

void midisd_write_channel_number(midisd_header_t hdr,
                                  unsigned int channel_number);

unsigned int midisd_strtoui(char *s);

#endif // MIDISD_COMMON_H
