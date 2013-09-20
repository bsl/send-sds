bin_PROGRAMS = midisd

midisd_LDFLAGS = $(shell pkg-config --cflags --libs alsa sndfile)

midisd_SOURCES = midisd_main.c \
                 midisd_cli.c \
                 midisd_command.c \
                 midisd_conf.c \
                 midisd_log.c \
                 midisd_dump.c \
                 midisd_convert.c \
                 midisd_rawmidi.c \
                 midisd_send.c \
                 midisd_receive.c \
                 midisd_sndfile.c


# midisd_LDADD = $(shell pkg-config --cflags --libs alsa sndfile)
# AM_LDFLAGS = -lasound -lsndfile