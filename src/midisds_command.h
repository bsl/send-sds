#ifndef MIDISDS_COMMAND_H
#define MIDISDS_COMMAND_H

typedef enum {
    MIDISDS_COMMAND_SEND,
    MIDISDS_COMMAND_RECEIVE,
    MIDISDS_COMMAND_DUMP,
    MIDISDS_COMMAND_INTERFACE_LIST,
    MIDISDS_COMMAND_UNKNOWN
} MIDISDS_COMMAND;

typedef struct {
    const MIDISDS_COMMAND cmd;
    const char *str;
    const char *desc;
} MIDISDS_COMMAND_INFO;

MIDISDS_COMMAND midisds_string_to_command(const char*);
int midisds_num_commands(void);
int midisds_num_supported_commands(void);
int midisds_supported_commands(char **buf, size_t buf_size);
MIDISDS_COMMAND_INFO get_command_info(const char *cmd);
int midisds_command_desc(char *cmd, char *buf, size_t buf_size);
int midisds_iflist(void);

#endif
