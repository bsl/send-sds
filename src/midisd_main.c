// midisd_main.c
// Brian Sorahan

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "midisd.h"
#include "midisd_cli.h"
#include "midisd_conf.h"
#include "midisd_command.h"
#include "midisd_common.h"
#include "midisd_convert.h"
#include "midisd_dump.h"
#include "midisd_receive.h"
#include "midisd_send.h"

void print_usage(void);

int main(int argc, char** argv) {
    const char *conf_file = "/etc/midisd.conf";
    midisd_log_trace("using %s for runtime configuration", conf_file);

    char *command_string;
    MIDISD_COMMAND command = MIDISD_COMMAND_UNKNOWN;
    midisd_send_file_options_t send_file_opts;
    int exit_code = 0;

    midisd_log_trace("checking arguments");
    if (argc < 2) {
        print_usage();
        exit(1);
    }

    if (midisd_read_conf_file(conf_file) == 0) {
        midisd_log_error("could not read %s", conf_file);
    } else {
        char *log_level = midisd_get_conf_for("log_level");
        midisd_set_log_level_str(log_level);
    }

    command_string = argv[1];
    command = midisd_string_to_command(command_string);

    switch(command) {
    case MIDISD_COMMAND_SEND:
        send_file_opts = midisd_parse_send_file_options(argc, argv);
        // TODO: error handling
        midisd_send_file(&send_file_opts);
        break;
    case MIDISD_COMMAND_INTERFACE_LIST:
        exit_code = midisd_iflist();
        break;
    case MIDISD_COMMAND_HELP:
        if (argc < 3) {
            midisd_help(MIDISD_COMMAND_HELP);
            exit_code = 1;
        } else {
            midisd_log_trace("getting help");
            exit_code = midisd_help(midisd_string_to_command(argv[2]));
        }
        break;
    case MIDISD_COMMAND_DUMP:
        if (argc < 3) {
            midisd_help(MIDISD_COMMAND_DUMP);
            exit_code = 1;
        } else {
            exit_code = midisd_dump(argv[2]);
        }
        break;
    case MIDISD_COMMAND_CONVERT:
        if (argc < 3) {
            midisd_help(MIDISD_COMMAND_CONVERT);
            exit_code = 1;
        } else {
            exit_code = midisd_convert_to_sds_temp(argv[2], NULL);
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
    int ncmds = midisd_num_supported_commands();
    const int BUF_SIZE = 200 * ncmds;
    char cmd_info_buf[BUF_SIZE];
    cmd_info_buf[0] = '\0';
    midisd_supported_commands(cmd_info_buf, (size_t) BUF_SIZE);
    printf("usage: midisd <command> [options]\n");
    printf("\ncommands:\n%s", cmd_info_buf);
}
