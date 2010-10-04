#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include <alsa/asoundlib.h>

#include "err.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define DUMP_HEADER_LENGTH 21
#define DATA_PACKET_LENGTH 127

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

static int
open_midi_interface(
    const char *device,
    snd_rawmidi_t **handle_in,
    snd_rawmidi_t **handle_out,
    err_t err
);

static int
open_sds_file(
    const char *filename,
    int *fd_r,
    err_t err
);

static int
sds_file_size_is_ok(
    int fd,
    err_t err
);

static int send_sds_file(
    snd_rawmidi_t *handle_in,
    snd_rawmidi_t *handle_out,
    int fd,
    err_t err
);

static int
send_sds_header(
    snd_rawmidi_t *handle_in,
    snd_rawmidi_t *handle_out,
    int fd,
    err_t err
);

static int
send_sds_data_packet(
    snd_rawmidi_t *handle_in,
    snd_rawmidi_t *handle_out,
    int fd,
    err_t err
);

static int
send_data(
    snd_rawmidi_t *handle_in,
    snd_rawmidi_t *handle_out,
    const char *data,
    size_t data_length,
    err_t err
);

static int
get_response(
    snd_rawmidi_t *handle_in,
    response_t *response,
    err_t err
);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char **argv)
{
    const char *device = "hw:37,0,0";
    const char *filename = "ok.sds";

    err_t err;
    int ret;
    int fd;
    snd_rawmidi_t *handle_in, *handle_out;

    (void)argc;
    (void)argv;

    err = err_create(256);
    ret = 1;
    fd = 0;
    handle_in  = NULL;
    handle_out = NULL;

    if (
        !open_midi_interface(device, &handle_in, &handle_out, err) ||
        !open_sds_file(filename, &fd, err)                         ||
        !sds_file_size_is_ok(fd, err)                              ||
        !send_sds_file(handle_in, handle_out, fd, err)
    ) {
        fprintf(stderr, "%s\n", err_get(err));
        goto end;
    }

    ret = 0;

end:
    if (fd) {
        close(fd);
    }

    if (handle_in) {
        snd_rawmidi_close(handle_in);
    }

    if (handle_out) {
        snd_rawmidi_close(handle_out);
    }

    return ret;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

static int
open_midi_interface(
    const char *device,
    snd_rawmidi_t **handle_in,
    snd_rawmidi_t **handle_out,
    err_t err
) {
    int r;

    r = snd_rawmidi_open(handle_in, handle_out, device, 0);
    if (r) {
        err_set2(err, "snd_rawmidi_open \"%s\": error %d", device, r);
        return 0;
    }

    return 1;
}

static int
open_sds_file(
    const char *filename,
    int *fd_r,
    err_t err
) {
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        err_set2(err, "open \"%s\": %s", filename, strerror(errno));
        return 0;
    }

    *fd_r = fd;

    return 1;
}

static int
sds_file_size_is_ok(
    int fd,
    err_t err
) {
    struct stat buf;

    if (fstat(fd, &buf) == -1) {
        err_set2(err, "stat: %s", strerror(errno));
        return 0;
    }

    if ((buf.st_size - DUMP_HEADER_LENGTH) % DATA_PACKET_LENGTH != 0) {
        err_set2(err, "bad file size (%d)", buf.st_size);
        return 0;
    }

    return 1;
}

static int
send_sds_file(
    snd_rawmidi_t *handle_in,
    snd_rawmidi_t *handle_out,
    int fd,
    err_t err
) {
    return
        send_sds_header(handle_in, handle_out, fd, err) &&
        send_sds_data_packet(handle_in, handle_out, fd, err);
}

static int
send_sds_header(
    snd_rawmidi_t *handle_in,
    snd_rawmidi_t *handle_out,
    int fd,
    err_t err
) {
    char buf[DUMP_HEADER_LENGTH];
    int r;

    r = read(fd, buf, DUMP_HEADER_LENGTH);
    if (r != DUMP_HEADER_LENGTH) {
        err_set2(err, "read: tried to read %d bytes, read %d", DUMP_HEADER_LENGTH, r);
        return 0;
    }

    return send_data(handle_in, handle_out, buf, DUMP_HEADER_LENGTH, err);
}

static int
send_sds_data_packet(
    snd_rawmidi_t *handle_in,
    snd_rawmidi_t *handle_out,
    int fd,
    err_t err
) {
    char buf[DATA_PACKET_LENGTH];
    int r;

    while (1) {
        r = read(fd, buf, DATA_PACKET_LENGTH);

        if (r == 0) {
            return 1;
        }

        if (r != DATA_PACKET_LENGTH) {
            err_set2(err, "read: tried to read %d bytes, read %d", DUMP_HEADER_LENGTH, r);
            return 0;
        }

        if (!send_data(handle_in, handle_out, buf, DATA_PACKET_LENGTH, err)) {
            return 0;
        }
    }
}

static int
send_data(
    snd_rawmidi_t *handle_in,
    snd_rawmidi_t *handle_out,
    const char *data,
    size_t data_length,
    err_t err
) {
    ssize_t r;
    response_t response;

    r = snd_rawmidi_write(handle_out, data, data_length);
    if ((size_t)r != data_length) {
        err_set2(err, "snd_rawmidi_write: tried to write %d bytes, wrote %d", data_length, r);
        return 0;
    }

    r = get_response(handle_in, &response, err);
    if (r && response == RESPONSE_ACK) {
        return 1;
    }

    return 0;
}

static int
get_response(
    snd_rawmidi_t *handle_in,
    response_t *response,
    err_t err
) {
    int done;
    unsigned char c, x;
    response_state_t state;
    ssize_t r;

    done = 0;
    state = STATE0;

    while (!done) {
        r = snd_rawmidi_read(handle_in, &c, 1);
        if (r != 1) {
            err_set2(err, "snd_rawmidi_read: failed to read 1 byte");
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
                state = (c < 16)
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
                state = (c < 0x80)
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
