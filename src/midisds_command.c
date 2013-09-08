#include <strings.h>
#include <stdlib.h>

#include "midisds_command.h"

#define MIDISDS_NUM_COMMANDS 5
// 1 less than members of enum because last one is "unknown"
#define MIDISDS_NUM_SUPPORTED_COMMANDS 4

MIDISDS_COMMAND_INFO MIDISDS_COMMAND_INFO_ARR[MIDISDS_NUM_COMMANDS] = {
    {
        MIDISDS_COMMAND_SEND,
        "send",
        "Send a sample to a device"
    },
    {
        MIDISDS_COMMAND_RECEIVE,
        "receive",
        "Receive a sample from a device"
    },
    {
        MIDISDS_COMMAND_DUMP,
        "dump",
        "Print info about a sample to stdout"
    },
    {
        MIDISDS_COMMAND_INTERFACE_LIST,
        "iflist",
        "List available midi devices (uses 'amidi -l')"
    },
    {
        MIDISDS_COMMAND_UNKNOWN,
        "unknown",
        "Unknown command"
    }
};

// ====================
// function definitions
// ====================

int midisds_num_commands(void) {
    return MIDISDS_NUM_COMMANDS;
}

int midisds_num_supported_commands(void) {
    return MIDISDS_NUM_SUPPORTED_COMMANDS;
}

MIDISDS_COMMAND_INFO get_command_info(const char *cmd) {
    int i;
    int ncmds = midisds_num_commands();
    int suppcmds = midisds_num_supported_commands();

    for (i = 0; i < ncmds; i++) {
        MIDISDS_COMMAND_INFO cmdinfo = MIDISDS_COMMAND_INFO_ARR[i];
        if (strcasecmp(cmdinfo.str, cmd) == 0) {
            return cmdinfo;
        }
    }

    // hack -- should return UNKNOWN
    return MIDISDS_COMMAND_INFO_ARR[suppcmds];
}

MIDISDS_COMMAND midisds_string_to_command(const char *str) {
    return get_command_info(str).cmd;
}

int midisds_supported_commands(char **buf, size_t buf_size) {
    int i;
    int suppcmds = midisds_num_supported_commands();

    if (buf_size < suppcmds) {
        return 0; // TODO: better error handling
    } else {
        // notice we skip "unknown"
        for (i = 0; i < suppcmds; i++) {
            buf[i] = MIDISDS_COMMAND_INFO_ARR[i].str;
        }
        return suppcmds;
    }
}

int midisds_command_desc(char *cmd, char *buf, size_t buf_size) {
    MIDISDS_COMMAND_INFO cmdinfo = get_command_info(cmd);

    if (buf_size < sizeof cmdinfo.desc) {
        return 0;
    } else {
        strcpy(buf, cmdinfo.desc);
        return 1;
    }
}

int midisds_iflist(void) {
    // I couldn't find a way to do this through rawmidi [bps]
    return system("amidi -l");
}

