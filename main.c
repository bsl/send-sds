#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "err.h"
#include "midi.h"
#include "sds.h"

#define VERSION "2010.10.23"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

typedef enum {
    STATE0,  /* seen [],             hoping for f0             */
    STATE1,  /* seen [f0]            hoping for 7e             */
    STATE2,  /* seen [f0,7e]         hoping for channel number */
    STATE3,  /* seen [f0,7e,CN]      hoping for 7{c,d,e,f}     */
    STATE4,  /* seen [f0,7e,CN,x]    hoping for packet number  */
    STATE5,  /* seen [f0,7e,CN,x,PN] hoping for f7             */
} response_state_t;

typedef enum {
    RESPONSE_ACK,
    RESPONSE_NAK,
    RESPONSE_CANCEL,
    RESPONSE_WAIT
} response_t;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

static void
display_usage(void);

static int
convert_channel_num(
    char *s,
    unsigned int *channel_num,
    err_t err
);

static int
convert_sample_num(
    char *s,
    unsigned int *channel_num,
    err_t err
);

static int
convert_string_to_unsigned_int(
    char *s,
    unsigned int *ui
);

static int
send_file(
    int fd,
    size_t file_size,
    midi_t midi,
    unsigned int channel_num,
    unsigned int sample_num,
    err_t err
);

static int
get_response(
    midi_t midi,
    unsigned int channel_num,
    unsigned int modded_packet_num,
    response_t *response,
    err_t err
);

static const char *
response_to_string(response_t response);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char **argv)
{
    int ret;
    char *device, *channel_string, *sample_string, *filename;
    unsigned int channel_num, sample_num;
    int fd;
    size_t file_size;
    err_t err;
    midi_t midi;

    ret            = 1;
    device         = NULL;
    filename       = NULL;
    channel_string = NULL;
    sample_string  = NULL;
    channel_num    = 0;
    fd             = 0;
    err            = err_create(256);
    midi           = NULL;

    if (argc != 1+4) {
        display_usage();
        goto end;
    }

    device         = argv[1];
    channel_string = argv[2];
    sample_string  = argv[3];
    filename       = argv[4];

    if (
        !convert_channel_num(channel_string, &channel_num, err) ||
        !convert_sample_num(sample_string, &sample_num, err)    ||
        !midi_open_interface(device, &midi, err)                ||
        !sds_open_file(filename, &fd, err)                      ||
        !sds_get_file_size(fd, &file_size, err)                 ||
        !sds_file_size_is_ok(file_size, err)                    ||
        !send_file(fd, file_size, midi, channel_num, sample_num, err)
    ) {
        fprintf(stderr, "%s\n", err_get(err));
        goto end;
    }

    ret = 0;

end:
    err_destroy(err);

    if (fd) {
        close(fd);
    }

    midi_close_interface(midi);

    return ret;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

static void
display_usage(void)
{
    fprintf(
        stderr,
        "send-sds " VERSION "\n"
        "usage: <alsa-device> <channel-num> <sample-num> <sds-filename>\n"
    );
}

static int
convert_channel_num(
    char *s,
    unsigned int *channel_num,
    err_t err
) {
    unsigned int ui;

    if (!convert_string_to_unsigned_int(s, &ui)) {
        err_set2(err, "invalid channel number \"%s\"", s);
        return 0;
    }

    if (ui > 15) {
        err_set2(err, "invalid channel number \"%s\", should be 0..15", s);
    }

    *channel_num = ui;
    return 1;
}

static int
convert_sample_num(
    char *s,
    unsigned int *sample_num,
    err_t err
) {
    unsigned int ui;

    if (!convert_string_to_unsigned_int(s, &ui)) {
        err_set2(err, "invalid sample number \"%s\"", s);
        return 0;
    }

    if (ui > 16383) {
        err_set2(err, "invalid sample number \"%s\", should be 0..16383", s);
    }

    *sample_num = ui;
    return 1;
}

static int
convert_string_to_unsigned_int(
    char *s,
    unsigned int *ui
) {
    char *endptr;
    unsigned int c;

    errno = 0;
    c = strtoul(s, &endptr, 0);

    if (
        errno   != 0    ||
        *s      == '\0' ||
        *endptr != '\0'
    ) {
        return 0;
    }

    *ui = c;
    return 1;
}

#define max(a,b) ((a) > (b) ? (a) : (b))

static int
send_file(
    int fd,
    size_t file_size,
    midi_t midi,
    unsigned int channel_num,
    unsigned int sample_num,
    err_t err
) {
    unsigned char buf[max(SDS_HEADER_LENGTH, SDS_PACKET_LENGTH)];
    response_t response;
    unsigned int num_packets, packet_num, modded_packet_num;

    if (!sds_read_header(fd, buf, sizeof(buf), err)) {
        return 0;
    }

    /* patch in channel number */
    buf[2] = (unsigned char)channel_num;

    /* patch in sample number */
    buf[4] =  sample_num       & 0x7f;
    buf[5] = (sample_num >> 7) & 0x7f;

    if (!midi_send(midi, buf, SDS_HEADER_LENGTH, err)) {
        return 0;
    }

    if (!get_response(midi, channel_num, 0, &response, err)) {
        return 0;
    }

    if (response != RESPONSE_ACK) {
        err_set2(
            err,
            "received %s instead of %s in response to dump header",
            response_to_string(response),
            response_to_string(RESPONSE_ACK)
        );
        return 0;
    }

    num_packets = sds_calc_num_packets(file_size);
    for (packet_num=0; packet_num < num_packets; ) {
        modded_packet_num = packet_num % 0x80;

        if (!sds_read_packet(fd, buf, sizeof(buf), err)) {
            return 0;
        }

        /* XXX patch channel number */
        /* XXX patch packet number */

        if (!midi_send(midi, buf, SDS_PACKET_LENGTH, err)) {
            return 0;
        }

        if (!get_response(midi, channel_num, modded_packet_num, &response, err)) {
            return 0;
        }

        if (response != RESPONSE_ACK) {
            err_set2(
                err,
                "received %s instead of %s in response to packet %d",
                response_to_string(response),
                response_to_string(RESPONSE_ACK),
                packet_num
            );
            return 0;
        }

        packet_num++;
    }

    return 1;
}

static int
get_response(
    midi_t midi,
    unsigned int channel_num,
    unsigned int modded_packet_num,
    response_t *response,
    err_t err
) {
    int done;
    unsigned char c, x;
    response_state_t state;

    done = 0;
    state = STATE0;

    while (!done) {
        if (!midi_read(midi, &c, err)) {
            return 0;
        }

        switch (state) {
            case STATE0:
                state = (c == 0xf0)
                    ? STATE1
                    : STATE0;
                break;

            case STATE1:
                state = (c == 0x7e)
                    ? STATE2
                    : STATE0;
                break;

            case STATE2:
                state = (c == channel_num)
                    ? STATE3
                    : STATE0;
                break;

            case STATE3:
                state = (c >= 0x7c && c <= 0x7f)
                    ? STATE4
                    : STATE0;
                x = c;
                break;

            case STATE4:
                state = (c == modded_packet_num)
                    ? STATE5
                    : STATE0;
                break;

            case STATE5:
                if (c == 0xf7) {
                    done = 1;

                    switch (x) {
                        case 0x7c: *response = RESPONSE_WAIT;
                        case 0x7d: *response = RESPONSE_CANCEL;
                        case 0x7e: *response = RESPONSE_NAK;
                        case 0x7f: *response = RESPONSE_ACK;
                    }

                    return 1;
                } else {
                    state = STATE0;
                }
                break;
        }
    }

    return 0;
}

static const char *
response_to_string(response_t response)
{
    switch (response) {
        case RESPONSE_ACK:    return "ACK";
        case RESPONSE_NAK:    return "NAK";
        case RESPONSE_CANCEL: return "CANCEL";
        case RESPONSE_WAIT:   return "WAIT";
    }

    return "UNKNOWN";
}
