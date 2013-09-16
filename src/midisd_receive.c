#include <stdio.h>
#include <alsa/asoundlib.h>

#include "midisd.h"
#include "midisd_common.h"
#include "midisd_log.h"
#include "midisd_rawmidi.h"

midisd_message_t midisd_receive(const midi_t *midi) {
    size_t hdr_read_count = 0;
    size_t hdr_len = midisd_header_length();
    size_t hdr_read_sz = hdr_len;
    size_t hdr_bytes_read = 0;
    midisd_header_t hdr;
    midisd_packet_t *pkts = NULL;
    unsigned int num_packets = 0;
    midisd_message_t msg;

    // Receive Dump Header
    while (hdr_read_count < hdr_len) {
        hdr_bytes_read = midisd_read(midi, hdr, hdr_read_sz);
        hdr_read_count += hdr_bytes_read;
        hdr_read_sz -= hdr_bytes_read;
    }

    midisd_copy_header(msg.hdr, hdr);
    msg.pkts = pkts;
    msg.num_packets = num_packets;
    return msg;
}

