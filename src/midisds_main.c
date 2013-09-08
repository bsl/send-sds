#include <stdio.h>
#include <stdlib.h>

#include "midisds.h"
#include "midisds_command.h"
#include "midisds_common.h"
#include "midisds_dump.h"
#include "midisds_receive.h"
#include "midisds_send.h"

static int print_usage(void);

int main(int argc, char** argv) {
    char *command_string;
    MIDISDS_COMMAND command = MIDISDS_COMMAND_UNKNOWN;

    if (argc < 2) {
        print_usage();
    }

    command_string = argv[1];
    command = midisds_string_to_command(command_string);

    switch(command) {
    case MIDISDS_COMMAND_INTERFACE_LIST:
        return midisds_iflist();
    default:
        return print_usage();
    }

    return 0;
}

static int print_usage(void) {
    int i;
    int ncmds = midisds_num_supported_commands();
    char *cmds_buf[ncmds];

    if (! midisds_supported_commands(cmds_buf, ncmds)) {
        return 1;
    } else {
        printf("usage: midisds <command> [options]\n");
        printf("\ncommands:\n");
        for (i = 0; i < ncmds; i++) {
            size_t desc_buf_size = 400;
            char desc[desc_buf_size];
            desc[0] = '\0';

            if (midisds_command_desc(cmds_buf[i], desc, desc_buf_size)) {
                printf("  %-12s %s\n", cmds_buf[i], desc);
            } else {
                // causes compiler warning about unsupported format specifier
                // fprintf(stderr, "Error getting command description: %m\n");
                return 2;
            }
        }
        printf("See 'midisds help <command>' "
               "for more help with a specific command.\n");
        exit(0);
    }
}
