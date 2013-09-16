#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "midisds_log.h"

static midisds_log_level log_level = midisds_log_level_debug;

typedef struct __log_level_info {
    midisds_log_level l;
    const char *str;
} midisds_log_level_info_t;

static midisds_log_level_info_t
log_level_info_arr[midisds_log_level_unknown] = {
    {
        midisds_log_level_error,
        "ERROR"
    },
    {
        midisds_log_level_warn,
        "WARN"
    },
    {
        midisds_log_level_info,
        "INFO"
    },
    {
        midisds_log_level_debug,
        "DEBUG"
    },
    {
        midisds_log_level_trace,
        "TRACE"
    }
};

static int should_log(midisds_log_level req);
static void make_log_message(const char *str, char *buf, \
                             midisds_log_level level);
static const char *log_level_to_string(midisds_log_level l);
static void log_message(midisds_log_level level, const char *fmt, va_list ap);
static midisds_log_level str_to_level(char *);

void midisds_log_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisds_log_level_error, fmt, ap);
    va_end(ap);
}

void midisds_log_warn(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisds_log_level_warn, fmt, ap);
    va_end(ap);
}

void midisds_log_info(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisds_log_level_info, fmt, ap);
    va_end(ap);
}

void midisds_log_debug(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisds_log_level_debug, fmt, ap);
    va_end(ap);
}

void midisds_log_trace(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisds_log_level_trace, fmt, ap);
    va_end(ap);
}

void midisds_set_log_level_str(char *l) {
    midisds_log_level lev = str_to_level(l);
    if (lev == midisds_log_level_unknown) {
        // default to 'error'
        log_level = midisds_log_level_error;
    } else {
        log_level = lev;
    }
}

void midisds_set_log_level(midisds_log_level l) {
    log_level = l;
}

static int should_log(midisds_log_level req) {
    return (req <= log_level);
}

static midisds_log_level str_to_level(char *l) {\
    int i;
    for (i = 0; i < midisds_log_level_unknown; i++) {
        midisds_log_level_info_t info = log_level_info_arr[i];
        if (strcasecmp(l, info.str)) {
            return info.l;
        }
    }
    return midisds_log_level_unknown;
}

static void make_log_message(const char *str, char *buf, \
                             midisds_log_level level) {
    time_t curtime = time(NULL);
    struct tm *tmptr = localtime(&curtime);

    char timebuf[200]; timebuf[0] = '\0';

    sprintf(timebuf, "%d:%d:%d", \
            tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);

    const char *log_level_str = log_level_to_string(level);
    sprintf(buf, "[%s] %s %s\n", log_level_str, timebuf, str);
}

static const char *log_level_to_string(midisds_log_level l) {
    int i;
    for (i = 0; i < midisds_log_level_unknown; i++) {
        midisds_log_level_info_t info = log_level_info_arr[i];
        if (l == info.l) {
            return info.str;
        }
    }
    return log_level_info_arr[i].str;
}

static void log_message(midisds_log_level level, const char *fmt, va_list ap) {
    char msgbuf[500];
    msgbuf[0] = '\0';

    char sbuf[500];
    sbuf[0] = '\0';

    vsprintf(sbuf, fmt, ap);

    if (should_log(level)) {
        make_log_message(sbuf, msgbuf, level);

        if (log_level < midisds_log_level_info) {
            printf("%s", msgbuf);
        } else {
            fprintf(stderr, "%s", msgbuf);
        }
    }
}

