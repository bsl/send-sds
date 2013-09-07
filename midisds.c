#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#include "midisds.h"
#include "midisds_common.h"
#include "midisds_dump.h"
#include "midisds_receive.h"
#include "midisds_send.h"

void print_usage_exit(void);

typedef enum {
    MIDISDS_UNKNOWN,
    MIDISDS_SEND,
    MIDISDS_RECEIVE,
    MIDISDS_DUMP
} MIDISDS_COMMAND;

int main(int argc, char** argv) {
    char *command_string;
    MIDISDS_COMMAND command;

    if (argc != 2) {
        print_usage_exit();
    }

    command_string = argv[1];

    if (strcasecmp(command_string, "send") == 0) {
        command = MIDISDS_SEND;
    } else if (strcasecmp(command_string, "receive") == 0) {
        command = MIDISDS_RECEIVE;
    } else if (strcasecmp(command_string, "dump") == 0) {
        command = MIDISDS_DUMP;
    } else {
        command = MIDISDS_UNKNOWN;
    }

    return 0;
}

void print_usage_exit(void) {
}
