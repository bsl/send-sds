#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "err.h"
#include "midi.h"
#include "sds.h"

/*
 * Size of the buffer for reading midi data.
 */
#define READ_PACKET_BUF_SIZE 1000

#define __TRACE_READ_DUMP_HEADER 0
#define __DEBUG_READ_DUMP_HEADER 1
#define __TRACE_READ_DUMP_HEADER_WAIT 0

#define __TRACE_READ_PACKET 0
#define __DEBUG_READ_PACKET 1
#define __TRACE_READ_PACKET_WAIT 0

#define SDS_RESPONSE_LENGTH 6



typedef enum {
    HEADER_STATE0,  /* seen [],                   hoping for f0 */
    HEADER_STATE1,  /* seen [f0]                  hoping for 7e */
    HEADER_STATE2,  /* seen [f0,7e]               hoping for channel number */
    HEADER_STATE3,  /* seen [f0,7e,CN]            hoping for 01,... */
    HEADER_STATE4   /* seen [f0,7e,CN,01,...]     hoping for f7 */
} receive_header_state_t;

typedef enum {
    PACKET_STATE0,  /* seen [],                   hoping for f0 */
    PACKET_STATE1,  /* seen [f0]                  hoping for 7e */
    PACKET_STATE2,  /* seen [f0,7e]               hoping for channel number */
    PACKET_STATE3,  /* seen [f0,7e,CN]            hoping for 01,... */
    PACKET_STATE4,  /* seen [f0,7e,CN,02,]        hoping for packet number */
    PACKET_STATE5   /* seen [f0,7e,CN,02,...]     reading waveform data, hoping for f7 */
} receive_packet_state_t;



static void
display_usage(void);

static int
receive_sample(int fd,
               midi_t midi,
               unsigned int sample_num,
               unsigned int channel_num,
               err_t err);

static int
read_dump_header(midi_t midi,
                 unsigned int sample_num,
                 unsigned int channel_num,
                 err_t err);

static int
read_packet(midi_t midi,
            unsigned int sample_num,
            unsigned int channel_num,
            unsigned int modded_packet_num,
            err_t err);

static int
send_response(midi_t midi,
              unsigned int channel_num,
              unsigned int packet_num,
              response_t response,
              err_t err);

int main(int argc, char **argv) {
    int ret;
    char *device, *channel_string, *sample_string, *filename;
    unsigned int channel_num, sample_num;
    int fd;
    err_t err;
    midi_t midi;

    ret = 1;
    device = NULL;
    filename = NULL;
    channel_string = NULL;
    sample_string = NULL;
    channel_num = 0;
    fd = 0;
    err = NULL;
    midi = NULL;

    if (argc != 1 + 4) {
        display_usage();
        goto end;
    }

    device = argv[1];
    channel_string = argv[2];
    sample_string = argv[3];
    filename = argv[4];

    if (! convert_channel_num(channel_string, &channel_num, err) ||
        ! convert_sample_num(sample_string, &sample_num, err)    ||
        ! midi_open_interface(device, &midi, err)                ||
        ! sds_open_file(filename, &fd, err)                      ||
        ! receive_sample(fd, midi, sample_num, channel_num, err)) {

        fprintf(stderr, "%s\n", err_get(err));
        goto end;
    }

end:
    err_destroy(err);

    if (fd) {
        close(fd);
    }

    midi_close_interface(midi);

    return ret;
}

static void
display_usage(void)
{
    fprintf(
        stderr,
        "receive-sds " VERSION "\n"
        "usage: <alsa-device> <channel-num> <sample-num> <sds-filename>\n"
    );
}

static int
receive_sample(int fd,
               midi_t midi,
               unsigned int sample_num,
               unsigned int channel_num,
               err_t err) {
    unsigned int num_packets, packet_num, modded_packet_num;

    packet_num = num_packets = modded_packet_num = 0;

    printf("Initiating handshake.\n");

    if (! read_dump_header(midi, sample_num, channel_num, err)) {
        return 0;
    } else {
        printf("Sending ACK\n");
        if (send_response(midi, channel_num, modded_packet_num, RESPONSE_ACK, err)) {
            modded_packet_num = ++packet_num % 128;
            printf("Handshake successful. Begin closeloop transfer.\n");
        }
    }

    int readingPackets = 1;
    while (readingPackets) {
        if (! read_packet(midi, sample_num, channel_num, modded_packet_num, err)) {
            readingPackets = 0;
            return 0;
        } else {
            printf("Sending ACK\n");
            if (send_response(midi, channel_num, modded_packet_num, RESPONSE_ACK, err)) {
                modded_packet_num = ++packet_num % 128;
            }
        }
    }

    return 1;
}

static int
read_dump_header(midi_t midi,
                 unsigned int sample_num,
                 unsigned int channel_num,
                 err_t err) {
    const char *indent = "    ";
    const char *trace = "[TRACE]";

    int done, bytes_read, i;
    unsigned char c;
    unsigned char buf[READ_PACKET_BUF_SIZE];
    receive_header_state_t state;

    done = bytes_read = 0;
    state = HEADER_STATE0;

    printf("%s%s Reading Dump Header\n", indent, trace);

    while (! done) {
        if (bytes_read == READ_PACKET_BUF_SIZE) {
            char strbuf[200]; strbuf[0] = '\0';
            sds_serialize_header(strbuf, buf);

            printf("%s%s Read Dump Header %s", indent, trace, strbuf);
            printf("\nMaximum buffer size reached (%d)\n", READ_PACKET_BUF_SIZE);
            break;
        }

        if (! midi_read(midi, &c)) {
            err_set2(err, "Could not read midi");
            return 0;
        } else {
        }

        switch(state) {
        case HEADER_STATE0:
            if (c == 0xf0) {
                state = HEADER_STATE1;
                buf[bytes_read++] = c;

                if (__TRACE_READ_DUMP_HEADER && __TRACE_READ_DUMP_HEADER_WAIT) {
                    printf("%s%s Read first byte of dump header\n", indent, trace);
                }
            }
            break;
        case HEADER_STATE1:
            if (c == 0x7e) {
                state = HEADER_STATE2;
                buf[bytes_read++] = c;

                if (__TRACE_READ_DUMP_HEADER) {
                    printf("%s%s Read second byte of dump header\n", indent, trace);
                }
            }
            break;
        case HEADER_STATE2:
            if (c == channel_num) {
                state = HEADER_STATE3;
                buf[bytes_read++] = c;

                if (__TRACE_READ_DUMP_HEADER) {
                    printf("%s%s Read channel num %d from dump header\n",
                           indent, trace, channel_num);
                }
            } else {
                if (__TRACE_READ_DUMP_HEADER) {
                    printf("%s%s Read unexpected channel num from dump header (%d)\n",
                           indent, trace, (int) c);
                }
            }
            break;
        case HEADER_STATE3:
            if (c == 0x01) {
                state = HEADER_STATE4;
                buf[bytes_read++] = c;

                if (__TRACE_READ_DUMP_HEADER) {
                    printf("%s%s Read fourth byte of dump header\n", indent, trace);
                }
            } else {
                if (__TRACE_READ_DUMP_HEADER) {
                    printf("[WARN] Read unexpected fourth byte (%02X) in dump header\n", c);
                }
            }
            break;
        case HEADER_STATE4:
            if (c == 0xf7) {
                done = 1;
                buf[bytes_read++] = c;

                if (__TRACE_READ_DUMP_HEADER) {
                    printf("%s%s Read last byte of dump header\n", indent, trace);
                    printf("%s%s Total bytes read = %d\n", indent, trace, bytes_read);
                }
                if (__DEBUG_READ_DUMP_HEADER) {
                    printf("Received Dump Header ");
                    for (i = 0; i < bytes_read; i++) {
                        printf("%02X ", buf[i]);
                    }
                    printf("\n");
                }
            } else {
                buf[bytes_read++] = c;
            }
            break;
        }
    }

    return 1;
}

static int
read_packet(midi_t midi,
            unsigned int sample_num,
            unsigned int channel_num,
            unsigned int modded_packet_num,
            err_t err) {
    const char *indent = "    ";
    const char *trace = "[TRACE]";

    int done, bytes_read, i, audio_bytes_read;
    char strbuf[500];
    unsigned char c;
    unsigned char buf[READ_PACKET_BUF_SIZE];
    receive_packet_state_t state;

    done = bytes_read = audio_bytes_read = 0;
    state = PACKET_STATE0;

    printf("Reading Packet %d\n", modded_packet_num);

    while (! done) {
        if (bytes_read == READ_PACKET_BUF_SIZE) {
            strbuf[0] = '\0';
            sds_serialize_packet(strbuf, buf, bytes_read);
            printf("Read Packet %d Yielded %s", modded_packet_num, strbuf);
            printf("Maximum buffer size reached (%d)\n", READ_PACKET_BUF_SIZE);
            break;
        }

        if (! midi_read(midi, &c)) {
            err_set2(err, "Could not read midi");
            return 0;
        } else {
        }

        switch(state) {
        case PACKET_STATE0:
            if (c == 0xf0) {
                state = PACKET_STATE1;
                buf[bytes_read++] = c;
                
                if (__TRACE_READ_PACKET && __TRACE_READ_PACKET_WAIT) {
                    printf("%s%s Read first byte of packet\n", indent, trace);
                }
            }
            break;
        case PACKET_STATE1:
            if (c == 0x7e) {
                state = PACKET_STATE2;
                buf[bytes_read++] = c;

                if (__TRACE_READ_PACKET) {
                    printf("%s%s Read second byte of packet\n", indent, trace);
                }
            }
            break;
        case PACKET_STATE2:
            if (c == channel_num) {
                state = PACKET_STATE3;
                buf[bytes_read++] = c;

                if (__TRACE_READ_PACKET) {
                    printf("%s%s Read channel num %d from packet\n",
                           indent, trace, channel_num);
                }
            } else {
                if (__TRACE_READ_PACKET) {
                    printf("%s%s Read unexpected channel num from packet (%d)\n",
                           indent, trace, (int) c);
                }
            }
            break;
        case PACKET_STATE3:
            if (c == 0x02) {
                state = PACKET_STATE4;
                buf[bytes_read++] = c;

                if (__TRACE_READ_PACKET) {
                    printf("%s%s Read fourth byte of packet\n", indent, trace);
                }
            } else {
                if (__TRACE_READ_PACKET) {
                    printf("Read unexpected byte (%02X) in packet\n", c);
                }
            }
            break;
        case PACKET_STATE4:
            if (c == modded_packet_num) {
                state = PACKET_STATE5;
                buf[bytes_read++] = c;

                if (__TRACE_READ_PACKET) {
                    printf("%s%s Read fourth byte of packet\n", indent, trace);
                }
            } else {
                if (__TRACE_READ_PACKET) {
                    printf("Read %d but expected %d in packet\n", c, modded_packet_num);
                }
            }
            break;
        case PACKET_STATE5:
            buf[bytes_read++] = c;

            if (audio_bytes_read < SDS_AUDIO_BYTES_PER_PACKET) {
                audio_bytes_read++;
            } else {
                if (c == 0xf7) {
                    done = 1;

                    if (__TRACE_READ_PACKET) {
                        printf("%s%s Read last byte of packet\n", indent, trace);
                        printf("%s%s Total bytes read = %d\n", indent, trace, bytes_read);
                    }
                    if (__DEBUG_READ_PACKET) {
                        strbuf[0] = '\0';
                        sds_serialize_packet(strbuf, buf, bytes_read);
                        printf("%s%s Total bytes read = %d\n", indent, trace, bytes_read);
                        printf("Received Packet %s\n", strbuf);
                    }
                }
            }
            break;
        }
    }

    return 1;
}

static int
send_response(midi_t midi,
              unsigned int channel_num,
              unsigned int packet_num,
              response_t response,
              err_t err) {
    unsigned char buf[SDS_RESPONSE_LENGTH];

    buf[0] = 0xf0;
    buf[1] = 0x7e;
    buf[2] = channel_num;

    switch(response) {
    case RESPONSE_ACK:
        buf[3] = 0x7f;
        break;
    case RESPONSE_NAK:
        buf[3] = 0x7e;
        break;
    case RESPONSE_CANCEL:
        buf[3] = 0x7d;
        break;
    case RESPONSE_WAIT:
        buf[3] = 0x7c;
        break;
    default:
        fprintf(stderr, "unrecognized response type %s\n", response_to_string(response));
        exit(180);
    }

    buf[4] = packet_num;
    buf[5] = 0xf7;

    if (! midi_send(midi, buf, SDS_RESPONSE_LENGTH, err)) {
        return 0;
    }

    return 1;
}

