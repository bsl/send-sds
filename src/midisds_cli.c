#include "midisds_send.h"
#include "midisds_cli.h"

midisds_send_file_options_t
midisds_parse_send_file_options(int argc, char **argv) {
    int opt;
    midisds_send_file_options_t opts;

    // defaults
    opts.sysex_channel = 0;
    opts.waveform_number = 1;

    while ((opt = getopt(argc, argv, "c:s:f:d:")) != -1) {
        switch (opt) {
        case 'c': // sysex channel
            opts.sysex_channel = midisds_strtoui(optarg);
            break;
        case 's': // sample number
            opts.waveform_number = midisds_strtoui(optarg);
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

