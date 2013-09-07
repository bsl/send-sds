#ifndef MIDISDS_SEND_H
#define MIDISDS_SEND_H

int midisds_send(const char *filename, int *fd);
/* int midisds_get_file_size(int fd, size_t *size); */
/* int midisds_file_size_is_ok(size_t size); */
/* unsigned int midisds_calc_num_packets(size_t size); */
/* int midisds_read_header(int fd, unsigned char *buf, size_t buf_size); */
/* int midisds_display_header(unsigned char *buf); */
/* int midisds_read_packet(int fd, unsigned char *buf, size_t buf_size); */

#endif
