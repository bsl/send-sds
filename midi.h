#ifndef MIDI_H
#define MIDI_H

#include <stdio.h>

#include "err.h"

/* make midi_t opaque */
struct midi;
typedef struct midi * midi_t;

int
midi_open_interface(
    const char *device,
    midi_t *midi,
    err_t err
);

void
midi_close_interface(
    midi_t midi
);

/* int midi_flush(midi_t midi); */

int
midi_send(
    midi_t midi,
    const unsigned char *data,
    size_t data_length,
    err_t err
);

int
midi_read(
    midi_t midi,
    unsigned char *byte
);

#endif
