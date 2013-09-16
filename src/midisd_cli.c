#include "midisd_send.h"
#include "midisd_cli.h"

midisd_send_file_options_t
midisd_parse_send_file_options(int argc, char **argv) {
    int opt;
    midisd_send_file_options_t opts;

    // defaults
    opts.sysex_channel = 0;
    opts.waveform_number = 1;

    while ((opt = getopt(argc, argv, "c:s:f:d:")) != -1) {
        switch (opt) {
        case 'c': // sysex channel
            opts.sysex_channel = midisd_strtoui(optarg);
            break;
        case 's': // sample number
            opts.waveform_number = midisd_strtoui(optarg);
            break;
        case 'f': // filename
            strcpy(opts.filename, optarg);
            break;
        case 'd': // alsa device
            strcpy(opts.device, optarg);
            break;
        }
    }

    return opts;
}

