#ifndef MIDISD_LOG_H
#define MIDISD_LOG_H

#include <stdio.h>

typedef enum {
    midisd_log_level_error,
    midisd_log_level_warn,
    midisd_log_level_info,
    midisd_log_level_debug,
    midisd_log_level_trace,
    midisd_log_level_unknown
} midisd_log_level;

void midisd_set_log_level(midisd_log_level level);
void midisd_set_log_level_str(char *l);
void midisd_log_error(const char *fmt, ...);
void midisd_log_warn(const char *fmt, ...);
void midisd_log_info(const char *fmt, ...);
void midisd_log_debug(const char *fmt, ...);
void midisd_log_trace(const char *fmt, ...);

#endif
