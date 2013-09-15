#define _XOPEN_SOURCE 500 /* string.h strdup */

#include <stdio.h>
#include <string.h>

#include <alsa/asoundlib.h>

#include "midisds_common.h"
#include "midisds_rawmidi.h"

midi_t midisds_open_interface(const char *device) {
    int r;
    midi_input_t *handle_in;
    midi_output_t *handle_out;
    midi_t midi;

    r = snd_rawmidi_open(&handle_in, &handle_out, device, 0);
    if (r) {
        char errmsg[100];
        errmsg[0] = '\0';
        sprintf(errmsg, "snd_rawmidi_open: error %d", r);
        midisds_log_error(errmsg);
    }

    // will need to call free in midi_close_interface
    midi.device = strdup(device);
    midi.handle_in = handle_in;
    midi.handle_out = handle_out;

    return midi;
}

midi_input_t *midisds_get_input(const midi_t *midi) {
    return midi->handle_in;
}

midi_output_t *midisds_get_output(const midi_t *midi) {
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

ssize_t midisds_send(const midi_t *midi, midisds_byte_t *data, size_t sz) {
    return snd_rawmidi_write(midi->handle_out, data, sz);
}

ssize_t midisds_read(const midi_t *midi, midisds_byte_t *buf, size_t sz) {
    return snd_rawmidi_read(midi->handle_in, buf, sz);
}

