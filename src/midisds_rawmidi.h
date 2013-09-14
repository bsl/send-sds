#ifndef MIDISDS_RAWMIDI_H
#define MIDISDS_RAWMIDI_H

#include <stdio.h>
#include <alsa/asoundlib.h>

#include "midisds_common.h"

typedef snd_rawmidi_t midi_input;
typedef snd_rawmidi_t midi_output;

typedef struct midi {
    char *device;
    midi_input *handle_in;
    midi_output *handle_out;
} midi_t;

midi_t midisds_open_interface(const char *device);
midi_input *midisds_get_input(const midi_t *);
midi_output *midisds_get_output(const midi_t *);
void midisds_close_interface(midi_t *midi);
ssize_t midisds_send(const midi_t *midi, midisds_byte *data, size_t sz);
ssize_t midisds_read(const midi_t *midi, midisds_byte *buf, size_t sz);

#endif
