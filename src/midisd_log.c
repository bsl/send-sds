#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "midisd_log.h"

static midisd_log_level log_level = midisd_log_level_error;

typedef struct __log_level_info {
    midisd_log_level l;
    const char *str;
} midisd_log_level_info_t;

static midisd_log_level_info_t
log_level_info_arr[midisd_log_level_unknown] = {
    {
        midisd_log_level_error,
        "ERROR"
    },
    {
        midisd_log_level_warn,
        "WARN"
    },
    {
        midisd_log_level_info,
        "INFO"
    },
    {
        midisd_log_level_debug,
        "DEBUG"
    },
    {
        midisd_log_level_trace,
        "TRACE"
    }
};

static int should_log(midisd_log_level req);
static void make_log_message(const char *str, char *buf, \
                             midisd_log_level level);
static const char *log_level_to_string(midisd_log_level l);
static void log_message(midisd_log_level level, const char *fmt, va_list ap);
static midisd_log_level str_to_level(char *);

void midisd_log_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisd_log_level_error, fmt, ap);
    va_end(ap);
}

void midisd_log_warn(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisd_log_level_warn, fmt, ap);
    va_end(ap);
}

void midisd_log_info(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisd_log_level_info, fmt, ap);
    va_end(ap);
}

void midisd_log_debug(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisd_log_level_debug, fmt, ap);
    va_end(ap);
}

void midisd_log_trace(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_message(midisd_log_level_trace, fmt, ap);
    va_end(ap);
}

void midisd_set_log_level_str(char *l) {
    midisd_log_level lev = str_to_level(l);
    if (lev == midisd_log_level_unknown) {
        // default to 'error'
        log_level = midisd_log_level_error;
    } else {
        log_level = lev;
    }
}

void midisd_set_log_level(midisd_log_level l) {
    log_level = l;
}

static int should_log(midisd_log_level req) {
    return (req <= log_level);
}

static midisd_log_level str_to_level(char *l) {\
    int i;
    for (i = 0; i < midisd_log_level_unknown; i++) {
        midisd_log_level_info_t info = log_level_info_arr[i];
        if (strcasecmp(l, info.str)) {
            return info.l;
        }
    }
    return midisd_log_level_unknown;
}

static void make_log_message(const char *str, char *buf, \
                             midisd_log_level level) {
    time_t curtime = time(NULL);
    struct tm *tmptr = localtime(&curtime);

    char timebuf[200]; timebuf[0] = '\0';

    sprintf(timebuf, "%d:%d:%d", \
            tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);

    const char *log_level_str = log_level_to_string(level);
    sprintf(buf, "[%s] %s %s\n", log_level_str, timebuf, str);
}

static const char *log_level_to_string(midisd_log_level l) {
    int i;
    for (i = 0; i < midisd_log_level_unknown; i++) {
        midisd_log_level_info_t info = log_level_info_arr[i];
        if (l == info.l) {
            return info.str;
        }
    }
    return log_level_info_arr[i].str;
}

static void log_message(midisd_log_level level, const char *fmt, va_list ap) {
    char msgbuf[500];
    msgbuf[0] = '\0';

    char sbuf[500];
    sbuf[0] = '\0';

    vsprintf(sbuf, fmt, ap);

    if (should_log(level)) {
        make_log_message(sbuf, msgbuf, level);

        if (log_level < midisd_log_level_info) {
            printf("%s", msgbuf);
        } else {
            fprintf(stderr, "%s", msgbuf);
        }
    }
}

