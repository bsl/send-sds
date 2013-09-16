// Runtime configuration for midisd.
// Brian Sorahan

#ifndef MIDISD_CONF_H
#define MIDISD_CONF_H

typedef struct __conf_entry {
    char key[100];
    char value[100];
} conf_entry_t;

void midisd_set_conf_for(const char *, const char *);
char *midisd_get_conf_for(const char *);
int midisd_read_conf_file(const char *filename);
int midisd_get_conf_str(char *buf);

#endif // MIDISD_CONF_H
