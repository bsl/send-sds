#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "midisds_command.h"

#define MIDISDS_NUM_COMMANDS 7
// 1 less than members of enum because last one is "unknown"
#define MIDISDS_NUM_SUPPORTED_COMMANDS 6

MIDISDS_COMMAND_INFO MIDISDS_COMMAND_INFO_ARR[MIDISDS_NUM_COMMANDS] = {
    {
        MIDISDS_COMMAND_SEND,
        "send",
        "Send a sample to a device",
        "usage: midisds send [options]\n\n"
        "options:\n"
        "  -d    ALSA midi device (get with midisds iflist)\n"
        "  -f    audio sample to send\n"
        "  -c    sysex channel to send on\n"
        "  -s    sample number to send to"
    },
    {
        MIDISDS_COMMAND_RECEIVE,
        "receive",
        "Receive a sample from a device",
        "usage: midisds receive [options] <output_file>\n\n"
        "options:\n"
        "  -d    ALSA midi device (get with midisds iflist)\n"
        "  -c    sysex channel to receive on\n"
        "  -s    sample number to receive"
    },
    {
        MIDISDS_COMMAND_DUMP,
        "dump",
        "Print info about a sample to stdout",
        "usage: midisds dump <input_file>"
    },
    {
        MIDISDS_COMMAND_CONVERT,
        "convert",
        "Convert a sample to sds format",
        "usage: midisds convert <input_file>"
    },
    {
        MIDISDS_COMMAND_INTERFACE_LIST,
        "iflist",
        "List available midi devices (uses 'amidi -l')",
        "usage: midisds iflist"
    },
    {
        MIDISDS_COMMAND_HELP,
        "help",
        "Get help with a particular command",
        "usage: midisds help <command>"
    },
    {
        MIDISDS_COMMAND_UNKNOWN,
        "unknown",
        "Unknown command",
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

MIDISDS_COMMAND_INFO midisds_get_command_info(const char *cmd) {
    int i;
    int ncmds = midisds_num_commands();
    int suppcmds = midisds_num_supported_commands();

    for (i = 0; i < ncmds; i++) {
        MIDISDS_COMMAND_INFO ci = MIDISDS_COMMAND_INFO_ARR[i];
        if (strcasecmp(ci.str, cmd) == 0) {
            return ci;
        }
    }

    // should return UNKNOWN
    return MIDISDS_COMMAND_INFO_ARR[suppcmds];
}

MIDISDS_COMMAND midisds_string_to_command(const char *str) {
    return midisds_get_command_info(str).cmd;
}

// Copy command information into buf
int midisds_supported_commands(char *buf, size_t buf_size) {
    int i;
    int suppcmds = midisds_num_supported_commands();
    char cmd_buf[buf_size / suppcmds];

    // notice we skip "unknown"
    for (i = 0; i < suppcmds; i++) {
        MIDISDS_COMMAND_INFO cmdinfo = MIDISDS_COMMAND_INFO_ARR[i];
        cmd_buf[0] = '\0';
        sprintf(cmd_buf, "  %-12s %s\n", cmdinfo.str, cmdinfo.desc);
        strcat(buf, cmd_buf);
    }

    return suppcmds;
}

int midisds_iflist(void) {
    // I couldn't find a way to do this through rawmidi [bps]
    return system("amidi -l");
}

// returns an exit code
int midisds_help(MIDISDS_COMMAND cmd) {
    MIDISDS_COMMAND_INFO info = MIDISDS_COMMAND_INFO_ARR[cmd];
    printf("%s\n%s\n", info.desc, info.help);
    return 0;
}
