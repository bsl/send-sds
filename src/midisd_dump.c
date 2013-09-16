#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#include <sndfile.h>

#include "midisd_common.h"
#include "midisd_dump.h"
#include "midisd_log.h"
#include "midisd_sndfile.h"

static const char *header_desc[MIDISD_HEADER_LENGTH] = {
    "", "", "cc", "", "sl", "sh", "ee", "pl", "pm", "ph",
    "gl", "gm", "gh", "hl", "hm", "hh", "il", "im", "ih", "jj", ""
};
// returns an exit code
int midisd_dump(const char *filename) {
    midisd_sndfile_t sf;
    midisd_log_info("dumping %s\n", filename);
    sf = midisd_sndfile_open(filename, MIDISD_SF_READ);

    printf("%-20s %s\n", "filename", filename);
    printf("%-20s %d\n", "frames", (int) sf.info->frames);
    printf("%-20s %d\n", "samplerate", sf.info->samplerate);
    printf("%-20s %d\n", "channels", sf.info->channels);

    char fmtbuf[400];
    fmtbuf[0] = '\0';
    midisd_get_format_info(&sf, fmtbuf);
    printf("%-20s %s\n", "format", fmtbuf);

    printf("%-20s %d\n", "sections", sf.info->sections);
    printf("%-20s %s\n", "seekable", sf.info->seekable ? "yes" : "no");

    // display the sds dump header (if there is one)
    if (midisd_sf_is_sds(&sf)) {
        int i;
        unsigned char hdrbuf[MIDISD_HEADER_LENGTH];
        int fd = open(filename, O_RDONLY);
        ssize_t hdr_bytes_read = read(fd, hdrbuf, \
                                      (size_t) MIDISD_HEADER_LENGTH);

        if (hdr_bytes_read != MIDISD_HEADER_LENGTH) {
            fprintf(stderr, "Could not read %d bytes from %s\n", \
                    MIDISD_HEADER_LENGTH, filename);
            return 4;
        } else {
            printf("%-20s", "Dump Header");
            for (i = 0; i < MIDISD_HEADER_LENGTH; i++) {
                printf(" %02X", (int) hdrbuf[i]);
            }
            printf("\n");

            printf("%-20s","");
            for (i = 0; i < MIDISD_HEADER_LENGTH; i++) {
                printf("%3s", header_desc[i]);
            }
            printf("\n");
        }
    }

    midisd_sndfile_close(&sf);

    return 0;
}
