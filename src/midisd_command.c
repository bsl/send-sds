#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "midisd_command.h"

#define MIDISD_NUM_COMMANDS 7
// 1 less than members of enum because last one is "unknown"
#define MIDISD_NUM_SUPPORTED_COMMANDS 6

MIDISD_COMMAND_INFO MIDISD_COMMAND_INFO_ARR[MIDISD_NUM_COMMANDS] = {
    {
        MIDISD_COMMAND_SEND,
        "send",
        "Send a sample to a device",
        "usage: midisd send [options]\n\n"
        "options:\n"
        "  -d    ALSA midi device (get with midisd iflist)\n"
        "  -f    audio sample to send\n"
        "  -c    sysex channel to send on\n"
        "  -s    sample number to send to"
    },
    {
        MIDISD_COMMAND_RECEIVE,
        "receive",
        "Receive a sample from a device",
        "usage: midisd receive [options] <output_file>\n\n"
        "options:\n"
        "  -d    ALSA midi device (get with midisd iflist)\n"
        "  -c    sysex channel to receive on\n"
        "  -s    sample number to receive"
    },
    {
        MIDISD_COMMAND_DUMP,
        "dump",
        "Print info about a sample to stdout",
        "usage: midisd dump <input_file>"
    },
    {
        MIDISD_COMMAND_CONVERT,
        "convert",
        "Convert a sample to sds format",
        "usage: midisd convert <input_file>"
    },
    {
        MIDISD_COMMAND_INTERFACE_LIST,
        "iflist",
        "List available midi devices (uses 'amidi -l')",
        "usage: midisd iflist"
    },
    {
        MIDISD_COMMAND_HELP,
        "help",
        "Get help with a particular command",
        "usage: midisd help <command>"
    },
    {
        MIDISD_COMMAND_UNKNOWN,
        "unknown",
        "Unknown command",
        "Unknown command"
    }
};

// ====================
// function definitions
// ====================

int midisd_num_commands(void) {
    return MIDISD_NUM_COMMANDS;
}

int midisd_num_supported_commands(void) {
    return MIDISD_NUM_SUPPORTED_COMMANDS;
}

MIDISD_COMMAND_INFO midisd_get_command_info(const char *cmd) {
    int i;
    int ncmds = midisd_num_commands();
    int suppcmds = midisd_num_supported_commands();

    for (i = 0; i < ncmds; i++) {
        MIDISD_COMMAND_INFO ci = MIDISD_COMMAND_INFO_ARR[i];
        if (strcasecmp(ci.str, cmd) == 0) {
            return ci;
        }
    }

    // should return UNKNOWN
    return MIDISD_COMMAND_INFO_ARR[suppcmds];
}

MIDISD_COMMAND midisd_string_to_command(const char *str) {
    return midisd_get_command_info(str).cmd;
}

// Copy command information into buf
int midisd_supported_commands(char *buf, size_t buf_size) {
    int i;
    int suppcmds = midisd_num_supported_commands();
    char cmd_buf[buf_size / suppcmds];

    // notice we skip "unknown"
    for (i = 0; i < suppcmds; i++) {
        MIDISD_COMMAND_INFO cmdinfo = MIDISD_COMMAND_INFO_ARR[i];
        cmd_buf[0] = '\0';
        sprintf(cmd_buf, "  %-12s %s\n", cmdinfo.str, cmdinfo.desc);
        strcat(buf, cmd_buf);
    }

    return suppcmds;
}

int midisd_iflist(void) {
    // I couldn't find a way to do this through rawmidi [bps]
    return system("amidi -l");
}

// returns an exit code
int midisd_help(MIDISD_COMMAND cmd) {
    MIDISD_COMMAND_INFO info = MIDISD_COMMAND_INFO_ARR[cmd];
    printf("%s\n%s\n", info.desc, info.help);
    return 0;
}
