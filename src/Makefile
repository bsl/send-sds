CC = gcc
CFLAGS = -I/usr/include/alsa -std=gnu99 -pedantic -W -Wall -Wno-variadic-macros -Wbad-function-cast -Wcast-align -Wcast-qual -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winline -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-compare -Wstrict-prototypes -Wundef -Wwrite-strings

# LDLIBS := -lasound -lsndfile
LDLIBS := $(shell pkg-config --cflags --libs alsa sndfile)

rm = rm -f

.PHONY: clean all

all: midisd

midisd: midisd_common.c \
		midisd_cli.c \
	    midisd_conf.c \
	    midisd_log.c \
	    midisd_dump.c \
        midisd_convert.c \
        midisd_rawmidi.c \
	    midisd_send.c \
		midisd_receive.c \
		midisd_sndfile.c \
		midisd_command.c
	$(CC) $(CFLAGS) -o $@ midisd_main.c $^ $(LDLIBS)

clean:
	-$(rm) *.o *~ midisd
