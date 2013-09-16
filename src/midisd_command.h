#ifndef MIDISD_COMMAND_H
#define MIDISD_COMMAND_H

#include <stdlib.h>

typedef enum {
    MIDISD_COMMAND_SEND,
    MIDISD_COMMAND_RECEIVE,
    MIDISD_COMMAND_DUMP,
    MIDISD_COMMAND_CONVERT,
    MIDISD_COMMAND_INTERFACE_LIST,
    MIDISD_COMMAND_HELP,
    MIDISD_COMMAND_UNKNOWN
} MIDISD_COMMAND;

typedef struct {
    const MIDISD_COMMAND cmd;
    const char *str;
    const char *desc;
    const char *help;
} MIDISD_COMMAND_INFO;

MIDISD_COMMAND midisd_string_to_command(const char*);
int midisd_num_commands(void);
int midisd_num_supported_commands(void);
int midisd_supported_commands(char *buf, size_t buf_size);
MIDISD_COMMAND_INFO midisd_get_command_info(const char *cmd);
int midisd_iflist(void);
int midisd_help(MIDISD_COMMAND cmd);

#endif
