// Runtime configuration for midisds.
// Brian Sorahan

#ifndef MIDISDS_CONF_H
#define MIDISDS_CONF_H

typedef struct __conf_entry {
    char key[100];
    char value[100];
} conf_entry_t;

void midisds_set_conf_for(const char *, const char *);
char *midisds_get_conf_for(const char *);
int midisds_read_conf_file(const char *filename);
int midisds_get_conf_str(char *buf);

#endif // MIDISDS_CONF_H
