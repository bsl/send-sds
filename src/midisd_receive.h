#ifndef MIDISD_RECEIVE_H
#define MIDISD_RECEIVE_H

#include "midisd_common.h"
#include "midisd_rawmidi.h"

midisd_message_t midisd_receive(const midi_t *midi);

#endif
