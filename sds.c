#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "sds.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

static int sds_read(int fd, size_t length, unsigned char *buf, size_t buf_size, err_t err);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int sds_open_file(const char *filename, int *fd_r, err_t err) {
    int fd;
    fd = open(filename, O_RDONLY);

    if (fd == -1) {
        err_set2(err, "open \"%s\": %s", filename, strerror(errno));
        return 0;
    }

    *fd_r = fd;
    return 1;
}

int sds_get_file_size(int fd, size_t *size, err_t err) {
    struct stat buf;

    if (fstat(fd, &buf) == -1) {
        err_set2(err, "stat: %s", strerror(errno));
        return 0;
    }

    *size = buf.st_size;
    return 1;
}

int sds_file_size_is_ok(size_t size, err_t err) {
    if ((size < SDS_HEADER_LENGTH)
        || ((size - SDS_HEADER_LENGTH) % SDS_PACKET_LENGTH != 0)) {
        err_set2(err, "bad file size (%d)", size);
        return 0;
    }

    return 1;
}

/* assumes sds_file_size_is_ok(size, ...) */
unsigned int sds_calc_num_packets(size_t size) {
    return (size - SDS_HEADER_LENGTH) / SDS_PACKET_LENGTH;
}

int sds_read_header(int fd, unsigned char *buf, size_t buf_size, err_t err) {
    return sds_read(fd, SDS_HEADER_LENGTH, buf, buf_size, err);
}

/* assumes sds_read_header has already read data into buf */
int sds_display_header(unsigned char* buf) {
    int i;

    printf("SDS HEADER: ");
    for (i = 0; i < SDS_HEADER_LENGTH; i++) {
        printf("%X ", buf[i]);
    }
    printf("\n");
    return 1;
}

int sds_read_packet(int fd, unsigned char *buf, size_t buf_size, err_t err) {
    return sds_read(fd, SDS_PACKET_LENGTH, buf, buf_size, err);
}

/* int sds_write_file(char* filename, unsigned char *buf, size_t buf_size) { */
/*     int header_done; */
/* } */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

static int sds_read(int fd, size_t length, unsigned char *buf,
                    size_t buf_size, err_t err) {
    ssize_t r;

    if (length > buf_size) {
        err_set2(err, "supposed to read %d bytes, buffer size only %d",
                 length, buf_size);
        return 0;
    }

    r = read(fd, buf, length);

    if ((size_t)r != length) {
        if (r == -1) {
            err_set2(err, "read: %s", strerror(errno));
        } else {
            err_set2(err, "read: tried to read %d bytes, actually read %d",
                     length, r);
        }

        return 0;
    }

    return 1;
}
