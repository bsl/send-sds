// midisds_main.c
// Brian Sorahan

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "midisds.h"
#include "midisds_command.h"
#include "midisds_common.h"
#include "midisds_convert.h"
#include "midisds_dump.h"
#include "midisds_receive.h"
#include "midisds_send.h"

void print_usage(void);

int main(int argc, char** argv) {
    char *command_string;
    MIDISDS_COMMAND command = MIDISDS_COMMAND_UNKNOWN;
    midisds_send_file_options_t send_file_opts;
    int exit_code = 0;

    if (argc < 2) {
        print_usage();
        exit(1);
    }

    command_string = argv[1];
    command = midisds_string_to_command(command_string);

    switch(command) {
    case MIDISDS_COMMAND_SEND:
        send_file_opts = midisds_parse_send_file_options(argc, argv);
        midisds_inspect_send_file_options(send_file_opts);
        break;
    case MIDISDS_COMMAND_INTERFACE_LIST:
        exit_code = midisds_iflist();
        break;
    case MIDISDS_COMMAND_HELP:
        if (argc < 3) {
            midisds_help(MIDISDS_COMMAND_HELP);
            exit_code = 1;
        } else {
            exit_code = midisds_help(midisds_string_to_command(argv[2]));
        }
        break;
    case MIDISDS_COMMAND_DUMP:
        if (argc < 3) {
            midisds_help(MIDISDS_COMMAND_DUMP);
            exit_code = 1;
        } else {
            exit_code = midisds_dump(argv[2]);
        }
        break;
    case MIDISDS_COMMAND_CONVERT:
        if (argc < 3) {
            midisds_help(MIDISDS_COMMAND_CONVERT);
            exit_code = 1;
        } else {
            exit_code = midisds_convert_to_sds_temp(argv[2], NULL);
        }
        break;
    default:
        print_usage();
        exit_code = 0;
    }

    return exit_code;
}

// Returns an exit code
void print_usage(void) {
    int ncmds = midisds_num_supported_commands();
    const int BUF_SIZE = 200 * ncmds;
    char cmd_info_buf[BUF_SIZE];
    cmd_info_buf[0] = '\0';
    midisds_supported_commands(cmd_info_buf, (size_t) BUF_SIZE);
    printf("usage: midisds <command> [options]\n");
    printf("\ncommands:\n%s", cmd_info_buf);
}
