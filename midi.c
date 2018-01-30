#define _XOPEN_SOURCE 500 /* string.h strdup */

#include "midi.h"

#include <stdio.h>
#include <string.h>

#include <alsa/asoundlib.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

struct midi {
    char *device;
    snd_rawmidi_t *handle_in;
    snd_rawmidi_t *handle_out;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int
midi_open_interface(
    const char *device,
    midi_t *midi_r,
    err_t err
) {
    int r;
    snd_rawmidi_t *handle_in, *handle_out;
    midi_t midi;

    r = snd_rawmidi_open(&handle_in, &handle_out, device, 0);
    if (r) {
        err_set2(err, "snd_rawmidi_open \"%s\": error %d", device, r);
        return 0;
    }

    midi             = malloc(sizeof(*midi));
    midi->device     = strdup(device);
    midi->handle_in  = handle_in;
    midi->handle_out = handle_out;

    *midi_r = midi;

    return 1;
}

void
midi_close_interface(
    midi_t midi
) {
    if (midi == NULL) {
        return;
    }

    free(midi->device);
    free(midi->handle_in);
    free(midi->handle_out);
    free(midi);
}

/* int */
/* midi_flush(midi_t midi) { */
/*     return (snd_rawmidi_drop(midi->handle_out) == 0) */
/*         && (snd_rawmidi_drop(midi->handle_in) == 0); */
/* } */

int
midi_send(
    midi_t midi,
    const unsigned char *data,
    size_t data_length,
    err_t err
) {
    ssize_t r;

    r = snd_rawmidi_write(midi->handle_out, data, data_length);
    if ((size_t)r != data_length) {
        err_set2(err, "snd_rawmidi_write: tried to write %d bytes, wrote %d", data_length, r);
        return 0;
    }

    return 1;
}

int
midi_read(
    midi_t midi,
    unsigned char *byte
) {
    ssize_t r;

    r = snd_rawmidi_read(midi->handle_in, byte, 1);
    if ((size_t)r != 1) {
        /* err_set2(err, "snd_rawmidi_read: tried to read 1 byte, read %d", r); */
        fprintf(stderr,
                "snd_rawmidi_read: tried to read 1 byte, read %d\n", (int) r);
        return 0;
    }

    return 1;
}
