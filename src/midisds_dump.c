#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#include <sndfile.h>

#include "midisds_dump.h"
#include "midisds_common.h"

int get_format_info(SF_INFO sf_info, char *buf);

// returns an exit code
int midisds_dump(const char *filename) {
    SF_INFO sfinfo;
    SNDFILE *sndfile;

    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    printf("%-20s %s\n", "filename", filename);
    printf("%-20s %d\n", "frames", (int) sfinfo.frames);
    printf("%-20s %d\n", "samplerate", sfinfo.samplerate);
    printf("%-20s %d\n", "channels", sfinfo.channels);

    char fmtbuf[400];
    fmtbuf[0] = '\0';
    get_format_info(sfinfo, fmtbuf);
    printf("%-20s %s\n", "format", fmtbuf);

    printf("%-20s %d\n", "sections", sfinfo.sections);
    printf("%-20s %s\n", "seekable", sfinfo.seekable ? "yes" : "no");

    sf_close(sndfile);

    // display the sds dump header (if there is one)
    if ((sfinfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_SDS) {
        int i;
        unsigned char hdrbuf[MIDISDS_HEADER_LENGTH];
        int fd = open(filename, O_RDONLY);
        ssize_t hdr_bytes_read = read(fd, hdrbuf, \
                                      (size_t) MIDISDS_HEADER_LENGTH);

        if (hdr_bytes_read != MIDISDS_HEADER_LENGTH) {
            fprintf(stderr, "Could not read %d bytes from %s\n", \
                    MIDISDS_HEADER_LENGTH, filename);
            return 4;
        } else {
            printf("%-20s", "Dump Header");
            for (i = 0; i < MIDISDS_HEADER_LENGTH; i++) {
                printf(" %X", (int) hdrbuf[i]);
            }
            printf("\n");
        }
    }

    return 0;
}

// There has *got* to be a better way! [bps]
int get_format_info(SF_INFO sf_info, char *buf) {
    SF_FORMAT_INFO fmtinfo;
    int major_count, subtype_count, m, s;

    sf_command(NULL, SFC_GET_FORMAT_MAJOR_COUNT, \
               &major_count, sizeof(int));
    sf_command(NULL, SFC_GET_FORMAT_SUBTYPE_COUNT, \
               &subtype_count, sizeof(int));

    // Get the name of the major format
    for (m = 0; m < major_count; m++) {
        fmtinfo.format = m;
        sf_command(NULL, SFC_GET_FORMAT_MAJOR, &fmtinfo, sizeof(fmtinfo));
        if (fmtinfo.format == (sf_info.format & SF_FORMAT_TYPEMASK)) {
            strcat(buf, fmtinfo.name);
        }
    }

    for (s = 0; s < subtype_count; s++) {
        fmtinfo.format = s;
        sf_command(NULL, SFC_GET_FORMAT_SUBTYPE, &fmtinfo, sizeof(fmtinfo));
        if (fmtinfo.format == (sf_info.format & SF_FORMAT_SUBMASK)) {
            strcat(buf, " ");
            strcat(buf, fmtinfo.name);
        }
    }

    return 1;
}
