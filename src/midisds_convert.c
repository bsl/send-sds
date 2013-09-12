#include <stdio.h>
#include <string.h>
#include <sndfile.h>

#include "midisds_convert.h"

#define FRAME_BUF_SIZE       512

static SF_INFO make_sds_info(SF_INFO input);

int midisds_convert_to_sds_temp(const char *filename) {
    SF_INFO sfinfo;
    SNDFILE *sndfile;

    sfinfo.format = 0;
    sndfile = sf_open(filename, SFM_READ, &sfinfo);

    // SDS format *must* be mono
    if (sfinfo.channels > 1) {
        fprintf(stderr, "File must be mono to convert to sds.");
        return 5;
    }

    char tmpnm[100];
    tmpnam(tmpnm);
    strcat(tmpnm, ".sds");
    printf("output file -> %s\n", tmpnm);

    SNDFILE *tmpsndfile;
    SF_INFO wrinfo = make_sds_info(sfinfo);
    tmpsndfile = sf_open(tmpnm, SFM_WRITE, &wrinfo);

    // copy the data
    double sfdata[FRAME_BUF_SIZE * sfinfo.channels];
    sf_count_t frames_read = 0;
    sf_count_t frames_written = 0;
    sf_count_t frame_buf_size = (sf_count_t) FRAME_BUF_SIZE;

    frames_read = sf_readf_double(sndfile, sfdata, frame_buf_size);
    frames_written = sf_writef_double(tmpsndfile, sfdata, frames_read);

    while (1) {
        frames_read = sf_readf_double(sndfile, sfdata, frame_buf_size);
        frames_written = sf_writef_double(tmpsndfile, sfdata, frames_read);

        if (frames_written != frames_read) {
            fprintf(stderr, "Error writing to %s\n", tmpnm);
        }

        if (frames_read != frame_buf_size) {
            break;
        }
    }

    sf_close(sndfile);
    sf_close(tmpsndfile);
    return 0;
}

static SF_INFO make_sds_info(SF_INFO input) {
    SF_INFO output;
    output.samplerate = input.samplerate;
    output.channels = input.channels;

    // calculate format where major is SF_FORMAT_SDS and
    // subtype is whatever the subtype of input is (as long as
    // this is passes sf_format_check)
    output.format = SF_FORMAT_SDS | (input.format & SF_FORMAT_SUBMASK);
    
    return output;
}
