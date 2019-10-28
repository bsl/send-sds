#include "sds.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define __SERIALIZE_INCLUDE_AUDIO_BYTES 1



static int
sds_read(
    int fd,
    size_t length,
    unsigned char *buf,
    size_t buf_size,
    err_t err
);



int
sds_open_file(
    const char *filename,
    int *fd_r,
    err_t err
) {
    int fd;

    fd = open(filename, O_RDWR | O_CREAT,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1) {
        err_set2(err, "open \"%s\": %s", filename, strerror(errno));
        return 0;
    }

    *fd_r = fd;

    return 1;
}

int
sds_get_file_size(
    int fd,
    size_t *size,
    err_t err
) {
    struct stat buf;

    if (fstat(fd, &buf) == -1) {
        err_set2(err, "stat: %s", strerror(errno));
        return 0;
    }

    *size = buf.st_size;

    return 1;
}

int
sds_file_size_is_ok(
    size_t size,
    err_t err
) {
    if (
        (size < SDS_HEADER_LENGTH) ||
        ((size - SDS_HEADER_LENGTH) % SDS_PACKET_LENGTH != 0)
    ) {
        err_set2(err, "bad file size (%d)", size);
        return 0;
    }

    return 1;
}

/* assumes sds_file_size_is_ok(size, ...) */
unsigned int
sds_calc_num_packets(
    size_t size
) {
    return (size - SDS_HEADER_LENGTH) / SDS_PACKET_LENGTH;
}

int
sds_read_header(
    int fd,
    unsigned char *buf,
    size_t buf_size,
    err_t err
) {
    return sds_read(fd, SDS_HEADER_LENGTH, buf, buf_size, err);
}

int
sds_serialize_header(char *str, unsigned char *buf) {
    int i;
    int chars = 0;
    char cstr[4]; cstr[0] = '\0';
    for (i = 0; i < SDS_HEADER_LENGTH; i++) {
        chars += sprintf(cstr, "%02X ", buf[i]);
        str = strcat(str, cstr);
    }
    // write null to the last space
    str[chars - 1] = '\0';
    return 1;
}

int
sds_read_packet(
    int fd,
    unsigned char *buf,
    size_t buf_size,
    err_t err
) {
    return sds_read(fd, SDS_PACKET_LENGTH, buf, buf_size, err);
}

int
sds_serialize_packet(char *str, unsigned char *buf, int packet_length) {
    int i;
    int chars = 0;
    char cstr[8]; cstr[0] = '\0';
    for (i = 0; i < 5; i++) {
        chars += sprintf(cstr, "%02X ", buf[i]);
        str = strcat(str, cstr);
    }

    if (__SERIALIZE_INCLUDE_AUDIO_BYTES) {
        for (i = 5; i < 125; i++) {
            chars += sprintf(cstr, "%02X ", buf[i]);
            str = strcat(str, cstr);
        }
    } else {
        chars += sprintf(cstr, " ... ");
        str = strcat(str, cstr);
    }

    for (i = 125; i < packet_length; i++) {
        chars += sprintf(cstr, "%02X ", buf[i]);
        str = strcat(str, cstr);
    }

    str[chars - 1] = '\0';
    return 1;
}



static int
sds_read(
    int fd,
    size_t length,
    unsigned char *buf,
    size_t buf_size,
    err_t err
) {
    ssize_t r;

    if (length > buf_size) {
        err_set2(err, "supposed to read %d bytes, buffer size only %d", length, buf_size);
        return 0;
    }

    r = read(fd, buf, length);
    if ((size_t)r != length) {
        if (r == -1) {
            err_set2(err, "read: %s", strerror(errno));
        } else {
            err_set2(err, "read: tried to read %d bytes, actually read %d", length, r);
        }

        return 0;
    }

    return 1;
}
