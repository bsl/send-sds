#include <string.h>

#include <sndfile.h>

#include "midisd_sndfile.h"

midisd_sndfile_t midisd_sndfile_open(const char *filename, int mode) {
    midisd_sndfile_t sf;
    sf.sf = sf_open(filename, mode, sf.info);
    return sf;
}

int midisd_sndfile_close(const midisd_sndfile_t *sf) {
    return sf_close(sf->sf);
}

// Stringify SF_INFO.format
int midisd_get_format_info(const midisd_sndfile_t *sf, char *buf) {
    SF_INFO *sf_info = sf->info;
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
        if (fmtinfo.format == (sf_info->format & SF_FORMAT_TYPEMASK)) {
            strcat(buf, fmtinfo.name);
        }
    }

    for (s = 0; s < subtype_count; s++) {
        fmtinfo.format = s;
        sf_command(NULL, SFC_GET_FORMAT_SUBTYPE, &fmtinfo, sizeof(fmtinfo));
        if (fmtinfo.format == (sf_info->format & SF_FORMAT_SUBMASK)) {
            strcat(buf, " ");
            strcat(buf, fmtinfo.name);
        }
    }

    return 1;
}

int midisd_sf_is_sds(const midisd_sndfile_t *sf) {
    return ((sf->info->format & SF_FORMAT_TYPEMASK) == SF_FORMAT_SDS);
}
