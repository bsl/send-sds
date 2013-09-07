#ifndef MIDISDS_COMMON_H
#define MIDISDS_COMMON_H

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

int midisds_open_file(const char *filename, int *fd);
int midisds_get_file_size(int fd, size_t *size);
int midisds_file_size_is_ok(size_t size);
unsigned int midisds_calc_num_packets(size_t size);
int midisds_read_header(int fd, unsigned char *buf, size_t buf_size);
int midisds_display_header(unsigned char *buf);
int midisds_read_packet(int fd, unsigned char *buf, size_t buf_size);

#endif // MIDISDS_COMMON_H
