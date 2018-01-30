#include "common.h"
#include "err.h"
#include "midi.h"
#include "sds.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Size of the buffer for reading midi data.
#define READ_PACKET_BUF_SIZE 1000



static void
display_usage(void);

static int
receive_sample(int fd,
               midi_t midi,
               unsigned int channel_num,
               err_t err);

static int
read_dump_header(midi_t midi,
                 unsigned char *buf,
                 unsigned int channel_num,
                 err_t err);

static int
read_packet(midi_t midi,
            unsigned char *buf,
            unsigned int channel_num,
            unsigned int modded_packet_num,
            err_t err);

static int
send_response(midi_t midi,
              unsigned int channel_num,
              unsigned int packet_num,
              response_type response,
              err_t err);

int main(int argc, char **argv) {
    int ret;
    char *device, *channel_string, *filename;
    unsigned int channel_num;
    int fd;
    err_t err;
    midi_t midi;

    ret = 1;
    device = NULL;
    filename = NULL;
    channel_string = NULL;
    channel_num = 0;
    fd = 0;
    err = err_create(256);
    midi = NULL;

    if (argc != 4) {
        display_usage();
        exit(249);
    }

    device = argv[1];
    channel_string = argv[2];
    filename = argv[3];

    if (! convert_channel_num(channel_string, &channel_num, err) ||
        ! midi_open_interface(device, &midi, err)                ||
        ! sds_open_file(filename, &fd, err)                      ||
        ! receive_sample(fd, midi, channel_num, err)) {
        fprintf(stderr, "Could not receive sample.\n");
    }

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
        "Copyright (c) 2018, brian\n"
        "Usage: receive-sds <alsa-device> <channel-num> <sds-filename>\n"
    );
}

static int
receive_sample(int fd,
               midi_t midi,
               unsigned int channel_num,
               err_t err) {
    unsigned char buf[READ_PACKET_BUF_SIZE];
    int packet_num, raw_packet_num;
    raw_packet_num = packet_num = 0;

    printf("Initiating handshake.\n");

    int header_bytes;
    header_bytes = read_dump_header(midi, buf, channel_num, err);
    if (header_bytes != SDS_HEADER_LENGTH) {
        return 0;
    } else {
        printf("Sending ACK...\n");
        if (send_response(midi, channel_num, packet_num, RESPONSE_ACK, err)) {
            write(fd, buf, header_bytes);
            printf("Handshake successful. Begin closeloop transfer.\n");
        }
    }

    int readingPackets, packet_size;
    readingPackets = 1;

    while (readingPackets) {
        packet_size = read_packet(midi, buf, channel_num, packet_num, err);
        if (packet_size != SDS_PACKET_LENGTH) {
            fprintf(stderr, "Received bad packet size %d\n", packet_size);
            readingPackets = 0;
            return 0;
        } else {
            printf("Sending ACK...\n");
            if (send_response(midi, channel_num, packet_num, RESPONSE_ACK, err)) {
                write(fd, buf, packet_size);
                raw_packet_num++;
                packet_num = raw_packet_num % 128;
            }
        }
    }

    return 1;
}

static int
read_dump_header(midi_t midi,
                 unsigned char *buf,
                 unsigned int channel_num,
                 err_t err) {
    int bytes_read;
    unsigned char c;
    char strbuf[200]; strbuf[0] = '\0';

    bytes_read = 0;

    printf("Reading Dump Header\n");

    while (bytes_read < SDS_HEADER_LENGTH) {
        if (! midi_read(midi, &c)) {
            err_set2(err, "Could not read midi");
            return 0;
        }

        switch(bytes_read) {
        case 0:
            if (c == 0xf0) {
                buf[bytes_read++] = c;
            }
            break;
        case 1:
            if (c == 0x7e) {
                buf[bytes_read++] = c;
            }
            break;
        case 2:
            if (c == channel_num) {
                buf[bytes_read++] = c;
            }
            break;
        case 3:
            if (c == 0x01) {
                buf[bytes_read++] = c;
            }
            break;
        case 20:
            if (c == 0xf7) {
                buf[bytes_read++] = c;
            } else {
                err_set2(err, "Got %02X as the last byte of dump header (expected F7).\n", c);
                return 0;
            }
            break;
        default:
            buf[bytes_read++] = c;
            break;
        }
    }

    sds_serialize_header(strbuf, buf);
    printf("Total bytes read = %d\n", bytes_read);
    printf("Received Dump Header %s\n", strbuf);
    return bytes_read;
}

/*
 * We consider a read_packet that times out to mean the dump operation
 * has completed successfully.
 */
static int
read_packet(midi_t midi,
            unsigned char *buf,
            unsigned int channel_num,
            unsigned int modded_packet_num,
            err_t err) {
    const int checksum_byte = 125;
    const time_t timeout_sec = 1;
    int raw_bytes_read, bytes_read;
    char strbuf[1000]; strbuf[0] = '\0';
    unsigned char c;
    unsigned int checksum = 0;

 /* sds_read_packet_start: */

    raw_bytes_read = bytes_read = 0;

    printf("Reading Packet %d\n", modded_packet_num);

    time_t start_time, now;
    start_time = time(NULL);

    while (bytes_read < SDS_PACKET_LENGTH) {
        now = time(NULL);

        if (now - start_time > timeout_sec) {
            printf("Done!\n");
            exit(0);
        }

        if (! midi_read(midi, &c)) {
            err_set2(err, "Could not read midi.");
            return 0;
        } else {
            raw_bytes_read++;
        }

        switch(bytes_read) {
        case 0:
            if (c == 0xf0) {
                buf[bytes_read++] = c;
            }
            break;
        case 1:
            if (c == 0x7e) {
                checksum ^= c;
                buf[bytes_read++] = c;
            }
            break;
        case 2:
            if (c == channel_num) {
                checksum ^= c;
                printf("channel_num = %d\n", channel_num);
                buf[bytes_read++] = c;
            }
            break;
        case 3:
            if (c == 0x02) {
                checksum ^= c;
                buf[bytes_read++] = c;
            }
            break;
        case 4:
            if (c == modded_packet_num) {
                checksum ^= c;
                printf("modded_packet_num = %d\n", modded_packet_num);
                buf[bytes_read++] = c;
            }
            break;
        case 125:
            buf[bytes_read++] = c;
            break;
        case 126:
            if (c == 0xf7) {
                buf[bytes_read++] = c;

                checksum &= 0x7f;
                if (checksum != buf[checksum_byte]) {
                    /* printf("Calculated checksum = %02X. Actual checksum = %02X.\n", */
                    /*        checksum, buf[checksum_byte]); */

                    // attempt to re-read sample
                    /* if (send_response(midi, channel_num, modded_packet_num, RESPONSE_NAK, err)) { */
                    /*     printf("Sent NAK...\n"); */
                    /*     goto sds_read_packet_start; */
                    /* } */
                } else {
                    printf("Checksums matched!\n");
                }
            }
            break;
        default:
            checksum ^= c;
            buf[bytes_read++] = c;
            break;
        }
    }

    sds_serialize_packet(strbuf, buf, bytes_read);
    /* printf("Total bytes read = %d\n", bytes_read); */
    /* printf("Received Packet %s\n", strbuf); */
    printf("Received Packet %d\n", modded_packet_num);

    return bytes_read;
}

static int
send_response(midi_t midi,
              unsigned int channel_num,
              unsigned int packet_num,
              response_type response,
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

