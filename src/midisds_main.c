#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#include "midisds.h"
#include "midisds_common.h"
#include "midisds_dump.h"
#include "midisds_receive.h"
#include "midisds_send.h"

typedef enum {
    MIDISDS_UNKNOWN,
    MIDISDS_SEND,
    MIDISDS_RECEIVE,
    MIDISDS_DUMP
} MIDISDS_COMMAND;

void print_usage_exit(void);
MIDISDS_COMMAND string_to_command(const char*);
const char *command_to_string(MIDISDS_COMMAND cmd);

int main(int argc, char** argv) {
    char *command_string;
    MIDISDS_COMMAND command = MIDISDS_UNKNOWN;

    if (argc != 2) {
        print_usage_exit();
    }

    command_string = argv[1];
    command = string_to_command(command_string);

    printf("executing %s\n", command_to_string(command));

    return 0;
}

MIDISDS_COMMAND string_to_command(const char *str) {
    if (strcasecmp(str, "send") == 0) {
        return MIDISDS_SEND;
    } else if (strcasecmp(str, "receive") == 0) {
        return MIDISDS_RECEIVE;
    } else if (strcasecmp(str, "dump") == 0) {
        return MIDISDS_DUMP;
    } else {
        return MIDISDS_UNKNOWN;
    }
}

const char *command_to_string(MIDISDS_COMMAND cmd) {
    switch (cmd) {
    case MIDISDS_SEND:
        return "send";
    case MIDISDS_RECEIVE:
        return "receive";
    case MIDISDS_DUMP:
        return "dump";
    default:
        return "unknown";
    }
}

void print_usage_exit(void) {
}
