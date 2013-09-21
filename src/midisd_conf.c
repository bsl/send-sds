// for strdup, getline
#define _XOPEN_SOURCE     700

#define MIDISD_MAX_CONF_ENTRIES      200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "midisd_conf.h"
#include "midisd_log.h"

static conf_entry_t conf[MIDISD_MAX_CONF_ENTRIES];
static int conf_entries = 0;

static int parse(char* ln, conf_entry_t *en);

void midisd_set_conf_for(const char *k, const char *v) {
    conf_entry_t en;
    strcpy(en.key, k);
    strcpy(en.value, v);
    memcpy(&conf[conf_entries], &en, sizeof(conf_entry_t));
    conf_entries++;
}

// Returns 0 for failure, 1 for success
char *midisd_get_conf_for(const char *k) {
    int i;

    for (i = 0; i < conf_entries; i++) {
        if (strcmp(k, conf[i].key) == 0) {
            return strdup(conf[i].value);
        }
    }

    return NULL;
}

// Returns 0 for failure, number of entries in the file on success
int midisd_read_conf_file(const char *filename) {
    static size_t max_line_size = 200;

    conf_entry_t en;
    int result = 0;
    ssize_t rd = 0;
    size_t line_sz = max_line_size;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        // file not found
        char error_msg[512]; error_msg[0] = '\0';
        sprintf(error_msg, "%s: in function '%s':\n%s:%d: "
                "Could not open %s", __FILE__, __FUNCTION__,
                __FILE__, __LINE__, filename);
        perror(error_msg);
        exit(1);
    }

    char *lnp = malloc(sizeof(char[max_line_size]));

    while (rd != -1) {
        rd = getline(&lnp, &line_sz, fp);
        if (parse(lnp, &en) == 0) {
            // TODO: error handling
        } else {
            midisd_set_conf_for(en.key, en.value);
            result++;
        }
    }

    free(lnp);
    return result;
}

// Returns 0 for failure, 1 for success
int midisd_get_conf_str(char *buf) {
    int i;
    char ln[200];

    for (i = 0; i < conf_entries; i++) {
        sprintf(ln, "%s=%s\n", conf[i].key, conf[i].value);
        buf = strcat(buf, ln);
    }
    return 1;
}

// Returns 0 for failure, 1 for success
// splits a string on the '=' character and
// stores a new conf entry
static int parse(char* ln, conf_entry_t *enp) {
    static const char *eq = "=";
    static const char *newline = "\n";

    conf_entry_t en;
    char *tok, k[100], v[100];
    k[0] = v[0] = '\0';

    tok = strtok(ln, eq);
    if (tok == NULL) {
        return 0;
    } else {
        strcpy(k, tok);
        tok = strtok(NULL, newline);

        if (tok == NULL) {
            return 0;
        } else {
            strcpy(v, tok);
        }
    }

    strcpy(en.key, k); strcpy(en.value, v);
    enp = memcpy(enp, &en, sizeof(conf_entry_t));
    return 1;
}

