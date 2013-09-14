#define _XOPEN_SOURCE 500 /* string.h strdup */

#include <stdio.h>
#include <string.h>

#include <alsa/asoundlib.h>

#include "midisds_common.h"
#include "midisds_rawmidi.h"

midi_t midisds_open_interface(const char *device) {
    int r;
    midi_input *handle_in;
    midi_output *handle_out;
    midi_t midi;

    r = snd_rawmidi_open(&handle_in, &handle_out, device, 0);
    if (r) {
        // TODO: handle error
    }

    // will need to call free in midi_close_interface
    midi.device = strdup(device);
    midi.handle_in = handle_in;
    midi.handle_out = handle_out;

    return midi;
}

midi_input *midisds_get_input(const midi_t *midi) {
    return midi->handle_in;
}

midi_output *midisds_get_output(const midi_t *midi) {
    return midi->handle_out;
}

void midisds_close_interface(midi_t *midi) {
    if (midi == NULL) {
        return;
    }

    free(midi->device);
    free(midi->handle_in);
    free(midi->handle_out);
    free(midi);
}

ssize_t midisds_send(const midi_t *midi, midisds_byte *data, size_t sz) {
    return snd_rawmidi_write(midi->handle_out, data, sz);
}

ssize_t midisds_read(const midi_t *midi, midisds_byte *buf, size_t sz) {
    return snd_rawmidi_read(midi->handle_in, buf, sz);
}

