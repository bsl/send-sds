#ifndef MIDISDS_H
#define MIDISDS_H

#define MIDISDS_0 // 0th version

#include <stdio.h>

/*
  From [http://www.blitter.com/~russtopia/MIDI/~jglatt/tech/sds.htm]
  ..................................................................

  Dump Header
  F0 7E cc 01 sl sh ee pl pm ph gl gm gh hl hm hh il im ih jj F7
  ...
  > cc represents the SysEx channel that the message is being sent upon.
  > There are 128 possible SysEx channels that a device can be set to
  > (ie, 0 to 127).
  > This allows various devices to be set to different SysEx channels along
  > the daisy-chain, and have the dump occur between 2 particular devices
  > with matching SysEx channels.
  ...
  > The sl sh is the 14-bit number (ie 0 to 16,384) of the waveform
  > which the receiver is requesting from the transmitter.
  > Most samplers number their internal waveforms from 0 to how ever
  > many waveforms there are. Note that the 14-bit sample number
  > is transmitted as 2 bytes where the first byte (sl) contains
  > bits 0 to 6 (with high bit clear), and the second byte (sh)
  > contains bits 7 to 13, right-justified (with high bit clear).
  ...
  > ee is the number of significant bits of the waveform.
  > For example, a 16-bit resolution waveform would have a 16 here.
  ...
  > pl pm ph is the sample period in nanoseconds
  > (ie, 1,000,000,000/sample rate in Hertz).
  > For example, a waveform sampled at 41667 Hertz will have a period of
  > 23,999 nanoseconds.
  > This value is transmitted as 3 bytes where pl is bits 0 to 6,
  > pm is bits 7 to 13 right-justified, and ph is bits 14 to 20
  > right-justified (ie, for a total of 20 bits of resolution)
  > with the high bit of all 3 bytes clear. So, our 23,999
  > (0x5DBF) becomes the 3 bytes 3F 3B 01.
  ...
  gl gm gh is the waveform length in words. (What this implies is that
  if you have 8-bit or less resolution, the waveform length will be
  half the number of sample points that you intend to dump.
  You always end up having to send an even number of points).
  ...
  > hl hm hh is the word offset (from 0, ie, the very first sample point
  > in the waveform) where the sustain loop starts.
  ...
  > il im ih is the word offset where the sustain loop ends
  > (ie, where the playback loops back to the sustain loop start).
  ...
  > jj is the looptype where 00 means "forward only" (most common)
  > and 01 means "backward/forward", and 7F means "no loop point"
  > (ie, the waveform is played through once only without looping).
  > Note that older MIDI samplers didn't support the 7F value for looptype.
  > For these older samplers, usually, if you set both th start and end loop
  > points to the same value as the waveform length, a sampler will consider
  > this to be a non-looped waveform. So to be safe, when you want to
  > indicate that a waveform is not to be looped, you should set looptype
  > to 7F, and set the start and end loop positions to the same value
  > as the waveform's length.
*/

#define MIDISDS_HEADER_LENGTH                  21
#define MIDISDS_PACKET_LENGTH                  127
#define MIDISDS_AUDIO_BYTES_PER_PACKET         120

// from libsndfile
#define MIDI_SDS_3BYTE_TO_INT_DECODE(x) \
    (((x) & 0x7F) | (((x) & 0x7F00) >> 1) | (((x) & 0x7F0000) >> 2))
#define MIDI_SDS_INT_TO_3BYTE_ENCODE(x) \
    (((x) & 0x7F) | (((x) << 1) & 0x7F00) | (((x) << 2) & 0x7F0000))

#define MIDI_SDS_MAX(a,b) ((a) > (b) ? (a) : (b))

// =========
// Functions
// =========

/* int midisds_open_file(const char *filename, int *fd); */
/* int midisds_get_file_size(int fd, size_t *size); */
/* int midisds_file_size_is_ok(size_t size); */
/* unsigned int midisds_calc_num_packets(size_t size); */
/* int midisds_read_header(int fd, unsigned char *buf, size_t buf_size); */
/* int midisds_display_header(unsigned char *buf); */
/* int midisds_read_packet(int fd, unsigned char *buf, size_t buf_size); */

#endif // MIDISDS_H
