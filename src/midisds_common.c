#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "midisds_common.h"
#include "midisds_send.h"

// ==========
// prototypes
// ==========

/* static int __midisds_read(int fd, size_t length, midisds_byte *buf, \ */
/*                           size_t buf_size); */

/* static int convert_channel_num(char *s, unsigned int *channel_num, err_t err); */

/* static int get_sample_num(unsigned char sl, unsigned char sh); */

/* static unsigned int midisds_calc_num_packets(size_t size); */

/* static int send_file(int fd, size_t file_size, midi_t midi, */
/*                      unsigned int channel_num, unsigned int sample_num, */
/*                      err_t err); */

/* static int get_response(midi_t midi, unsigned int channel_num, \ */
/*                         unsigned int modded_packet_num, \ */
/*                         response_t *response, err_t err); */

/* static const char * response_to_string(response_t response); */

// =========
// functions
// =========

ssize_t midisds_header_length(void) {
    return (size_t) MIDISDS_HEADER_LENGTH;
}

ssize_t midisds_packet_length(void) {
    return (size_t) MIDISDS_PACKET_LENGTH;
}

ssize_t midisds_audio_bytes_per_packet(void) {
    return (size_t) MIDISDS_AUDIO_BYTES_PER_PACKET;
}

midisds_header_t *midisds_get_header(midisds_message_t *msg) {
    return &msg->hdr;
}

void midisds_copy_header(midisds_header_t dest, midisds_header_t src) {
    ssize_t i;
    ssize_t hdr_len = midisds_header_length();
    for(i = 0; i < hdr_len; i++) {
        dest[i] = src[i];
    }
}

// Write the sysex channel number to midisds_header
void midisds_write_channel_number(midisds_header_t hdr, \
                                  unsigned int channel_number) {
    hdr[2] = (unsigned char) channel_number;
}

unsigned int midisds_strtoui(char *s) {
    return strtoul(s, NULL, 0);
}

/* midisds_message midisds_read_file(const char *filename, int *fd_r) { */
/*     int fd; */
/*     fd = open(filename, O_RDONLY); */

/*     if (fd == -1) { */
/*         return NULL; */
/*     } */

/*     *fd_r = fd; */
/*     return NULL; */
/* } */

/* assumes midisds_file_size_is_ok(size, ...) */
/* static unsigned int midisds_calc_num_packets(size_t size) { */
/*     return (size - MIDISDS_HEADER_LENGTH) / MIDISDS_PACKET_LENGTH; */
/* } */

/* int midisds_read_header(int fd, unsigned char *buf, size_t buf_size) { */
/*     return __midisds_read(fd, MIDISDS_HEADER_LENGTH, buf, buf_size); */
/* } */

/* assumes midisds_read_header has already read data into buf */
/* int midisds_display_header(unsigned char* buf) { */
/*     int i; */

/*     printf("MIDISDS HEADER: "); */
/*     for (i = 0; i < MIDISDS_HEADER_LENGTH; i++) { */
/*         printf("%X ", buf[i]); */
/*     } */
/*     printf("\n"); */
/*     return 1; */
/* } */

/* int midisds_read_packet(int fd, unsigned char *buf, size_t buf_size) { */
/*     return __midisds_read(fd, MIDISDS_PACKET_LENGTH, buf, buf_size); */
/* } */

// ========================
// Private static functions
// ========================

/* static int convert_channel_num(char *s, unsigned int *channel_num, err_t err) { */
/*     unsigned int ui; */

/*     if (!convert_string_to_unsigned_int(s, &ui)) { */
/*         err_set2(err, "invalid channel number \"%s\"", s); */
/*         return 0; */
/*     } */

/*     if (ui > 15) { */
/*         err_set2(err, "invalid channel number \"%s\", should be 0..15", s); */
/*     } */

/*     *channel_num = ui; */
/*     return 1; */
/* } */

/* static int get_sample_num(unsigned char sl, unsigned char sh) { */
/*     unsigned int ui; */

/*     // TODO: better error handling */
/*     if (ui > 16383) { */
/*         // error invalid sample num */
/*         return 0; */
/*     } else { */
/*         return ui; */
/*     } */
/* } */

/*
 * TODO: Improve error handling by refactoring err_t
 */
/* static int send_file(int fd, size_t file_size, midi_t midi, */
/*                      unsigned int channel_num, unsigned int sample_num, */
/*                      err_t err) { */
/*     unsigned char buf[max(MIDISDS_HEADER_LENGTH, MIDISDS_PACKET_LENGTH)]; */
/*     response_t response; */
/*     unsigned int num_packets, packet_num, modded_packet_num; */

/*     if (!midisds_read_header(fd, buf, sizeof(buf), err)) { */
/*         return 0; */
/*     } else { */
/*         midisds_display_header(buf); */
/*     } */

/*     /\* patch in channel number *\/ */
/*     buf[2] = (unsigned char)channel_num; */

/*     /\* patch in sample number *\/ */
/*     buf[4] =  sample_num       & 0x7f; */
/*     buf[5] = (sample_num >> 7) & 0x7f; */

/*     if (!midisds_send(midi, buf, MIDISDS_HEADER_LENGTH)) { */
/*         return 0; */
/*     } */

/*     if (!get_response(midi, channel_num, 0, &response)) { */
/*         return 0; */
/*     } */

/*     if (response != RESPONSE_ACK) { */
/*         err_set2(err, "received %s instead of %s in response to dump header", */
/*                  response_to_string(response), */
/*                  response_to_string(RESPONSE_ACK)); */
/*         return 0; */
/*     } */

/*     num_packets = midisds_calc_num_packets(file_size); */
/*     for (packet_num=0; packet_num < num_packets; ) { */
/*         modded_packet_num = packet_num % 0x80; */

/*         if (!midisds_read_packet(fd, buf, sizeof(buf), err)) { */
/*             return 0; */
/*         } */

/*         /\* XXX patch channel number *\/ */
/*         /\* XXX patch packet number *\/ */

/*         if (!midisds_send(midi, buf, MIDISDS_PACKET_LENGTH)) { */
/*             return 0; */
/*         } */

/*         if (!get_response(midi, channel_num, */
/*                           modded_packet_num, &response, err)) { */
/*             return 0; */
/*         } */

/*         if (response != RESPONSE_ACK) { */
/*             err_set2(err, "received %s instead of %s in response to packet %d", */
/*                      response_to_string(response), */
/*                      response_to_string(RESPONSE_ACK), */
/*                      packet_num); */
/*             return 0; */
/*         } */

/*         packet_num++; */
/*     } */

/*     return 1; */
/* } */

/* static int get_response(midi_t midi, unsigned int channel_num, */
/*                         unsigned int modded_packet_num, */
/*                         response_t *response, err_t err) { */
/*     int done; */
/*     unsigned char c, x; */
/*     response_state_t state; */

/*     done = 0; */
/*     state = STATE0; */

/*     while (!done) { */
/*         if (!midi_read(midi, &c, err)) { */
/*             return 0; */
/*         } */

/*         switch (state) { */
/*         case STATE0: */
/*             state = (c == 0xf0) ? STATE1 : STATE0; */
/*             break; */

/*         case STATE1: */
/*             state = (c == 0x7e) ? STATE2 : STATE0; */
/*             break; */

/*         case STATE2: */
/*             state = (c == channel_num) ? STATE3 : STATE0; */
/*             break; */

/*         case STATE3: */
/*             state = (c >= 0x7c && c <= 0x7f) ? STATE4 : STATE0; */
/*             x = c; */
/*             break; */

/*         case STATE4: */
/*             state = (c == modded_packet_num) ? STATE5 : STATE0; */
/*             break; */

/*         case STATE5: */
/*             if (c == 0xf7) { */
/*                 done = 1; */

/*                 switch (x) { */
/*                 case 0x7c: *response = RESPONSE_WAIT; */
/*                 case 0x7d: *response = RESPONSE_CANCEL; */
/*                 case 0x7e: *response = RESPONSE_NAK; */
/*                 case 0x7f: *response = RESPONSE_ACK; */
/*                 } */

/*                 return 1; */
/*             } else { */
/*                 state = STATE0; */
/*             } */
/*             break; */
/*         } */
/*     } */

/*     return 0; */
/* } */

/* static const char * response_to_string(response_t response) { */
/*     switch (response) { */
/*     case RESPONSE_ACK:    return "ACK"; */
/*     case RESPONSE_NAK:    return "NAK"; */
/*     case RESPONSE_CANCEL: return "CANCEL"; */
/*     case RESPONSE_WAIT:   return "WAIT"; */
/*     } */

/*     return "UNKNOWN"; */
/* } */

/* static int __midisds_read(int fd, size_t len, midisds_byte *buf, */
/*                           size_t buf_size) { */
/*     ssize_t r; */

/*     if (len > buf_size) { */
/*         return 0; */
/*     } */

/*     r = read(fd, buf, len); */

/*     if ((size_t)r != len) { */
/*         if (r == -1) { */
/*         } else { */
/*         } */

/*         return 0; */
/*     } */

/*     return 1; */
/* } */
