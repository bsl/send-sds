#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

#include "midisds_dump.h"
#include "midisds_common.h"

// returns an exit code
int midisds_dump(const char *filename) {
    SF_INFO sfinfo;
    SNDFILE *sndfile;

    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    printf("%-20s %s\n", "file", filename);
    printf("%-20s %d\n", "frames", (int) sfinfo.frames);
    printf("%-20s %d\n", "samplerate", sfinfo.samplerate);
    printf("%-20s %d\n", "channels", sfinfo.channels);

    SF_FORMAT_INFO fmtinfo;
    fmtinfo.format = (int) sfinfo.format;
    if (sf_command(sndfile, SFC_GET_FORMAT_INFO,
                   &fmtinfo, sizeof(fmtinfo)) == 0) {
        printf("%-20s %s\n", "format", fmtinfo.name);
    } else {
        printf("%s\n", sf_strerror(sndfile));
        return 4;
    }

    printf("%-20s %d\n", "sections", sfinfo.sections);
    printf("%-20s %s\n", "seekable", sfinfo.seekable ? "yes" : "no");

    sf_close(sndfile);
    return 0;
}

