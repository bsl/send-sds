#include <stdio.h>
#include <alsa/asoundlib.h>

#include "sds.h"

#define BUF_SIZE    256

void usage_and_exit(void);

int main(int argc, char** argv) {
    int open_result;
    char *device;
    snd_rawmidi_t *midi_in;
    snd_rawmidi_t *midi_out;

    if (argc != 3) {
        usage_and_exit();
    }

    device = argv[1];
    open_result = snd_rawmidi_open(&midi_in, &midi_out, device, 0);

    if (open_result) {
        fprintf(stderr, "Could not open device %s: %d\n", device, open_result);
        exit(3);
    }

    return 0;
}

void usage_and_exit() {
    fprintf(stderr, "usage: receive-sds <alsa-device> <filename>\n");
    exit(2);
}
