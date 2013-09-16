#ifndef MIDISD_RAWMIDI_H
#define MIDISD_RAWMIDI_H

#include <stdio.h>
#include <alsa/asoundlib.h>

#include "midisd_common.h"

typedef snd_rawmidi_t midi_input_t;
typedef snd_rawmidi_t midi_output_t;

typedef struct midi {
    char *device;
    midi_input_t *handle_in;
    midi_output_t *handle_out;
} midi_t;

midi_t midisd_open_interface(const char *device);
midi_input_t *midisd_get_input(const midi_t *);
midi_output_t *midisd_get_output(const midi_t *);
void midisd_close_interface(midi_t *midi);
ssize_t midisd_send(const midi_t *midi, midisd_byte_t *data, size_t sz);
ssize_t midisd_read(const midi_t *midi, midisd_byte_t *buf, size_t sz);

#endif
