#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sndfile.h>

#include "err.h"
#include "midisds_rawmidi.h"
#include "midisds_common.h"

int midisds_send_file(const char *filename, int *fd) {
    SF_INFO sfinfo;
    SNDFILE *sndfile;

    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if ((sfinfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_SDS) {
    } else {
        return 3;
    }
}
