#ifndef MIDISD_CLI_H
#define MIDISD_CLI_H

#include "midisd_send.h"

midisd_send_file_options_t
midisd_parse_send_file_options(int argc, char **argv);

#endif
