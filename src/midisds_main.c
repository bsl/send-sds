#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#include "midisds.h"
#include "midisds_common.h"
#include "midisds_dump.h"
#include "midisds_receive.h"
#include "midisds_send.h"

#define MIDISDS_NUM_COMMANDS              5
// 1 less than members of enum because last one is "unknown"
#define MIDISDS_NUM_SUPPORTED_COMMANDS    4

typedef enum {
    MIDISDS_COMMAND_SEND,
    MIDISDS_COMMAND_RECEIVE,
    MIDISDS_COMMAND_DUMP,
    MIDISDS_COMMAND_INTERFACE_LIST,
    MIDISDS_COMMAND_UNKNOWN
} MIDISDS_COMMAND;

const struct {
    const MIDISDS_COMMAND cmd;
    const char *str;
} MIDISDS_COMMAND_MAP[MIDISDS_NUM_COMMANDS] = {
    { MIDISDS_COMMAND_SEND, "send" },
    { MIDISDS_COMMAND_RECEIVE, "receive" },
    { MIDISDS_COMMAND_DUMP, "dump" },
    { MIDISDS_COMMAND_INTERFACE_LIST, "iflist" },
    { MIDISDS_COMMAND_UNKNOWN, "unknown" }
};

static void print_usage_exit(void);
static MIDISDS_COMMAND string_to_command(const char*);
/* static const char *command_to_string(MIDISDS_COMMAND cmd); */
static int midisds_supported_commands(char **buf, size_t buf_size);
static int midisds_iflist(void);

int main(int argc, char** argv) {
    char *command_string;
    MIDISDS_COMMAND command = MIDISDS_COMMAND_UNKNOWN;

    if (argc < 2) {
        print_usage_exit();
    }

    command_string = argv[1];
    command = string_to_command(command_string);

    switch(command) {
    case MIDISDS_COMMAND_INTERFACE_LIST:
        return midisds_iflist();
    default:
        print_usage_exit();
    }

    return 0;
}

// ====================================
// mapping between commands and strings
// ====================================

static MIDISDS_COMMAND string_to_command(const char *str) {
    int i;
    for (i = 0; i < MIDISDS_NUM_COMMANDS; i++) {
        if (strcasecmp(str, MIDISDS_COMMAND_MAP[i].str) == 0) {
            return MIDISDS_COMMAND_MAP[i].cmd;
        }
    }
    return MIDISDS_COMMAND_UNKNOWN;
}

/* static const char *command_to_string(MIDISDS_COMMAND cmd) { */
/*     int i; */
/*     for (i = 0; i < MIDISDS_NUM_COMMANDS; i++) { */
/*         if (MIDISDS_COMMAND_MAP[i].cmd == cmd) { */
/*             return MIDISDS_COMMAND_MAP[i].str; */
/*         } */
/*     } */
/*     return "unknown"; */
/* } */

static int midisds_supported_commands(char **buf, size_t buf_size) {
    int i;

    if (buf_size < MIDISDS_NUM_SUPPORTED_COMMANDS) {
        return 0; // TODO: better error handling
    } else {
        // notice we skip "unknown"
        for (i = 0; i < MIDISDS_NUM_SUPPORTED_COMMANDS; i++) {
            buf[i] = MIDISDS_COMMAND_MAP[i].str;
        }
        return MIDISDS_NUM_SUPPORTED_COMMANDS;
    }
}

static int midisds_iflist(void) {
    return system("amidi -l");
}

static void print_usage_exit(void) {
    int i;
    char *cmds_buf[MIDISDS_NUM_SUPPORTED_COMMANDS];

    if (! midisds_supported_commands(cmds_buf,
                                     MIDISDS_NUM_SUPPORTED_COMMANDS)) {
        exit(1); // TODO: better error handling
    } else {
        printf("usage: midisds command [options]\n");
        printf("supported commands:\n");
        for (i = 0; i < MIDISDS_NUM_SUPPORTED_COMMANDS; i++) {
            printf("* %s\n", cmds_buf[i]);
        }
        exit(0);
    }
}
