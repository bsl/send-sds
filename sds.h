#ifndef __SDS_H__
#define __SDS_H__

/*
 * Unless otherwise noted, I am using
 * [http://www.blitter.com/~russtopia/MIDI/~jglatt/tech/sds.htm]
 * as my spec for MIDI SDS.
 * I believe this document actually comes from the MMA spec.
 * [bps]
 */

#include <stdio.h>

#include "err.h"

/*
  Dump Header
  F0 7E cc 01 sl sh ee pl pm ph gl gm gh hl hm hh il im ih jj F7

  Where
  > ... cc represents the SysEx channel that the message is being sent upon.

  and
  > There are 128 possible SysEx channels that a device can be set to
  > (ie, 0 to 127).
  > This allows various devices to be set to different SysEx channels along
  > the daisy-chain, and have the dump occur between 2 particular devices
  > with matching SysEx channels.
*/
#define SDS_HEADER_LENGTH              0x15

#define SDS_PACKET_LENGTH              0x7F

#define SDS_AUDIO_BYTES_PER_PACKET     0x78

int sds_open_file(const char *filename, int *fd, err_t err);

int sds_get_file_size(int fd, size_t *size, err_t err);

int sds_file_size_is_ok(size_t size, err_t err);

unsigned int sds_calc_num_packets(size_t size);

int sds_read_header(int fd, unsigned char *buf, size_t buf_size, err_t err);

int sds_display_header(unsigned char *buf);

int sds_read_packet(int fd, unsigned char *buf, size_t buf_size, err_t err);

#define max(a,b) ((a) > (b) ? (a) : (b))

#endif
