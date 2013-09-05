#ifndef __SDS_H__
#define __SDS_H__

#include <stdio.h>

#include "err.h"

#define SDS_HEADER_LENGTH            21
#define SDS_PACKET_LENGTH            127
#define SDS_AUDIO_BYTES_PER_BLOCK    120

int sds_open_file(const char *filename, int *fd, err_t err);

int sds_get_file_size(int fd, size_t *size, err_t err);

int sds_file_size_is_ok(size_t size, err_t err);

unsigned int sds_calc_num_packets(size_t size);

int sds_read_header(int fd, unsigned char *buf, size_t buf_size, err_t err);

int sds_display_header(unsigned char *buf);

int sds_read_packet(int fd, unsigned char *buf, size_t buf_size, err_t err);

#endif
