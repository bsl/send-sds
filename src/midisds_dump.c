#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

#include "midisds_dump.h"
#include "midisds_common.h"

// returns an exit code
int midisds_dump(const char *filename) {
    SF_INFO sfinfo;
    sf_open(filename, SFM_READ, &sfinfo);
    printf("%-20s %s\n", "file", filename);
    printf("%-20s %d\n", "frames", (int) sfinfo.frames);
    return 0;
}
