#ifndef MIDISDS_SNDFILE_H
#define MIDISDS_SNDFILE_H

#include <sndfile.h>

#define MIDISDS_SF_READ         SFM_READ

typedef struct {
    SNDFILE *sf;
    SF_INFO *info;
} midisds_sndfile_t;

midisds_sndfile_t midisds_sndfile_open(const char *filename, int mode);
int midisds_sndfile_close(const midisds_sndfile_t *sf);
int midisds_get_format_info(const midisds_sndfile_t *sf, char *buf);
int midisds_sf_is_sds(const midisds_sndfile_t *sf);

#endif
