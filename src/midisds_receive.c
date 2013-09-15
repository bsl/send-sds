#include <stdio.h>
#include <alsa/asoundlib.h>

#include "midisds.h"
#include "midisds_common.h"
#include "midisds_log.h"
#include "midisds_rawmidi.h"

midisds_message_t midisds_receive(const midi_t *midi) {
    size_t hdr_read_count = 0;
    size_t hdr_len = midisds_header_length();
    size_t hdr_read_sz = hdr_len;
    size_t hdr_bytes_read = 0;
    midisds_header_t hdr;
    midisds_packet_t *pkts = NULL;
    unsigned int num_packets = 0;
    midisds_message_t msg;

    // Receive Dump Header
    while (hdr_read_count < hdr_len) {
        hdr_bytes_read = midisds_read(midi, hdr, hdr_read_sz);
        hdr_read_count += hdr_bytes_read;
        hdr_read_sz -= hdr_bytes_read;
    }

    midisds_copy_header(msg.hdr, hdr);
    msg.pkts = pkts;
    msg.num_packets = num_packets;
    return msg;
}

