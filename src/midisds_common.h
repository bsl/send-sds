#ifndef MIDISDS_COMMON_H
#define MIDISDS_COMMON_H

#include <stdio.h>

#include "midisds_err.h"

// ====
// defs
// ====

#define MIDISDS_HEADER_LENGTH                  21
#define MIDISDS_PACKET_LENGTH                  127
#define MIDISDS_AUDIO_BYTES_PER_PACKET         120

// jacked from libsndfile/src/sds.c
#define MIDI_SDS_3BYTE_TO_INT_DECODE(x) \
    (((x) & 0x7F) | (((x) & 0x7F00) >> 1) | (((x) & 0x7F0000) >> 2))
#define MIDI_SDS_INT_TO_3BYTE_ENCODE(x) \
    (((x) & 0x7F) | (((x) << 1) & 0x7F00) | (((x) << 2) & 0x7F0000))

#define MIDI_SDS_MAX(a,b) ((a) > (b) ? (a) : (b))

// =====
// types
// =====

// midisds_byte
typedef unsigned char midisds_byte;

// midisds_header
typedef midisds_byte midisds_header[MIDISDS_HEADER_LENGTH];

// midisds_packet
typedef midisds_byte midisds_packet[MIDISDS_PACKET_LENGTH];

// midisds_message
typedef struct {
    midisds_header *hdr;
    midisds_packet *pkts;
    unsigned int num_packets;
} midisds_message;

// =========
// functions
// =========

void midisds_write_channel_number(midisds_header hdr, \
                                  unsigned int channel_number);

/* midisds_message midisds_read_file(const char *filename, int *fd_r); */

size_t midisds_header_length(void);
size_t midisds_packet_length(void);
size_t midisds_audio_bytes_per_packet(void);

#endif // MIDISDS_COMMON_H
