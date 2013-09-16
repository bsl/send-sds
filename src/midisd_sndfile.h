#ifndef MIDISD_SNDFILE_H
#define MIDISD_SNDFILE_H

#include <sndfile.h>

#define MIDISD_SF_READ         SFM_READ

typedef struct {
    SNDFILE *sf;
    SF_INFO *info;
} midisd_sndfile_t;

midisd_sndfile_t midisd_sndfile_open(const char *filename, int mode);
int midisd_sndfile_close(const midisd_sndfile_t *sf);
int midisd_get_format_info(const midisd_sndfile_t *sf, char *buf);
int midisd_sf_is_sds(const midisd_sndfile_t *sf);

#endif
