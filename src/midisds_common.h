#ifndef MIDISDS_COMMON_H
#define MIDISDS_COMMON_H

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

// midi_sds_header
typedef struct {
    int sysex_channel;
    short waveform;
    short significant_bits;
    int sample_period; // measured in ns
} midi_sds_header;

// midi_sds_packet
typedef struct {
    midi_sds_header hdr; // to get significant bits
    unsigned char *buf;
    size_t buf_size;
} midi_sds_packet;

// midi_sds_message
typedef struct {
    midi_sds_header hdr;
    midi_sds_packet *pkts;
    int num_packets;
} midi_sds_message;

// =========
// functions
// =========

// Returns
int midisds_parse_header(const unsigned char *buf, size_t buf_size,
                         midi_sds_header *hdr);

int midisds_parse_packet(const unsigned char *buf, size_t buf_size,
                         midi_sds_packet *pkt);

/* Seems like these could just be private static functions */
/* int midisds_get_file_size(int fd, size_t *size); */
/* int midisds_file_size_is_ok(size_t size); */
/* unsigned int midisds_calc_num_packets(size_t size); */
/* int midisds_read_header(int fd, unsigned char *buf, size_t buf_size); */
/* int midisds_display_header(unsigned char *buf); */
/* int midisds_read_packet(int fd, unsigned char *buf, size_t buf_size); */

#endif // MIDISDS_COMMON_H
