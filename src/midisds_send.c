// For getopt
#define _XOPEN_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sndfile.h>

#include "midisds_common.h"
#include "midisds_convert.h"
#include "midisds_log.h"
#include "midisds_rawmidi.h"
#include "midisds_send.h"
#include "midisds_sndfile.h"

static midisds_message_t get_message_from_file(char *filename);
static int get_header_from_file(int fd, midisds_header_t hdr);
static ssize_t get_packets_from_file(int fd, midisds_message_t msg);

// Send an sds message to a device
// TODO: send something other than the header
ssize_t midisds_send_msg(const midi_t *midi,
                         midisds_send_message_options_t *options) {
    midisds_message_t *msg = options->msg;
    midisds_header_t *hdr = midisds_get_header(msg);
    ssize_t header_bytes_sent = midisds_send_header(midi, hdr);
    return header_bytes_sent;
}

// returns bytes sent
ssize_t midisds_send_header(const midi_t *midi, midisds_header_t *hdr) {
    ssize_t hdr_len = midisds_header_length();
    ssize_t bytes_sent = 0;

    // write sysex_channel and waveform_number into the header
    bytes_sent = midisds_send(midi, *hdr, hdr_len);

    if (hdr_len != bytes_sent) {
        char msgbuf[200];
        msgbuf[0] = '\0';

        sprintf(msgbuf, \
                "Only able to send %d bytes of header", (int) bytes_sent);

        midisds_log_error(msgbuf);
    } else {
        midisds_log_info("Successfully sent Dump Header");
    }

    return bytes_sent;
}

// Send an sds file to a device
ssize_t midisds_send_file(midisds_send_file_options_t *options) {
    ssize_t bytes_sent;
    midi_t midi = midisds_open_interface(options->device);
    midisds_message_t msg;
    midisds_send_message_options_t msgopts;
    midisds_sndfile_t sf = midisds_sndfile_open(options->filename,
                                                MIDISDS_SF_READ);

    // We have an sds file -- no conversion necessary
    if (midisds_sf_is_sds(&sf)) {
        msg = get_message_from_file(options->filename);
        msgopts.msg = &msg;
        msgopts.sysex_channel = options->sysex_channel;
        msgopts.waveform_number = options->waveform_number;
        bytes_sent = midisds_send_msg(&midi, &msgopts);
        midisds_close_interface(&midi);
    } else { // convert to sds, then send
        char tmpfilename[100];
        tmpfilename[0] = '\0';
        // TODO: error handling
        midisds_convert_to_sds_temp(options->filename, tmpfilename);
        strcpy(options->filename, tmpfilename);
        return midisds_send_file(options);
    }

    return bytes_sent;
}

void midisds_inspect_send_file_options(midisds_send_file_options_t opts) {
    printf("%-20s %s\n", "device", opts.device);
    printf("%-20s %s\n", "filename", opts.filename);
    printf("%-20s %d\n", "sysex_channel", opts.sysex_channel);
    printf("%-20s %d\n", "waveform_number", opts.waveform_number);
}

static midisds_message_t get_message_from_file(char *filename) {
    midisds_message_t msg;

    int fd = open(filename, O_RDONLY);

    get_header_from_file(fd, msg.hdr);
    msg.num_packets = get_packets_from_file(fd, msg);
    return msg;
}

static int get_header_from_file(int fd, midisds_header_t hdr) {
    ssize_t hdr_len = midisds_header_length();
    ssize_t byte_count;

    // read header
    byte_count = read(fd, &hdr, hdr_len);
    if (byte_count != hdr_len) {
        midisds_log_error("could not read header from file");
        return 0;
    }

    return 1;
}

// Returns the number of packets read
static ssize_t get_packets_from_file(int fd, midisds_message_t msg) {
    ssize_t pak_len = midisds_packet_length();
    ssize_t byte_count = 0, num_paks = 0;

    byte_count = read(fd, (msg.pkts + num_paks++), pak_len);
    while (byte_count == pak_len) {
        byte_count = read(fd, (msg.pkts + num_paks++), pak_len);
    }

    if (byte_count != 0) {
        midisds_log_error("Message may contain incomplete packet");
    }

    return num_paks;
}
