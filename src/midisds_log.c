#include <stdio.h>
#include <string.h>
#include <time.h>

#include "midisds_log.h"

static midisds_log_level log_level = midisds_log_level_error;

static int should_log(midisds_log_level req);
static void make_log_message(char *str, char *buf, midisds_log_level level);
static void log_level_to_string(midisds_log_level l, char *buf);
static void log_message(char *str, midisds_log_level level);

void midisds_log_error(char *str) {
    log_message(str, midisds_log_level_error);
}

void midisds_log_warn(char *str) {
    log_message(str, midisds_log_level_warn);
}

void midisds_log_info(char *str) {
    log_message(str, midisds_log_level_info);
}

void midisds_log_debug(char *str) {
    log_message(str, midisds_log_level_debug);
}

void midisds_log_trace(char *str) {
    log_message(str, midisds_log_level_trace);
}

void midisds_set_log_level(midisds_log_level l) {
    log_level = l;
}

static int should_log(midisds_log_level req) {
    return (req <= log_level);
}

static void make_log_message(char *str, char *buf, midisds_log_level level) {
    time_t curtime = time(NULL);
    struct tm *tmptr = localtime(&curtime);
    char timebuf[200];
    char levelbuf[10];
    sprintf(timebuf, "%d:%d:%d", \
            tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);
    log_level_to_string(level, levelbuf);
    sprintf(buf, "[%s] %s %s\n", levelbuf, timebuf, str);
}

static void log_level_to_string(midisds_log_level l, char *buf) {
    switch(l) {
    case midisds_log_level_error:
        sprintf(buf, "%s", "ERROR");
        break;
    case midisds_log_level_warn:
        sprintf(buf, "%s", "WARN");
        break;
    case midisds_log_level_info:
        sprintf(buf, "%s", "INFO");
        break;
    case midisds_log_level_debug:
        sprintf(buf, "%s", "DEBUG");
        break;
    case midisds_log_level_trace:
        sprintf(buf, "%s", "TRACE");
        break;
    default:
        break;
    }
}

static void log_message(char *str, midisds_log_level level) {
    char msgbuf[500];

    if (should_log(level)) {
        make_log_message(str, msgbuf, level);

        if (log_level < midisds_log_level_info) {
            printf("%s", msgbuf);
        } else {
            fprintf(stderr, "%s", msgbuf);
        }
    }
}

