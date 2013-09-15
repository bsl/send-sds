CC = gcc
CFLAGS = -I/usr/include/alsa -std=gnu99 -pedantic -W -Wall -Wno-variadic-macros -Wbad-function-cast -Wcast-align -Wcast-qual -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winline -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-compare -Wstrict-prototypes -Wundef -Wwrite-strings

# LDLIBS := -lasound -lsndfile
LDLIBS := $(shell pkg-config --cflags --libs alsa sndfile)

rm = rm -f

.PHONY: clean all

all: midisds

midisds: midisds_common.c \
		 midisds_cli.c \
		 midisds_conf.c \
	     midisds_log.c \
	     midisds_dump.c \
         midisds_convert.c \
         midisds_rawmidi.c \
		 midisds_send.c \
		 midisds_receive.c \
		 midisds_sndfile.c \
		 midisds_command.c
	$(CC) $(CFLAGS) -o $@ midisds_main.c $^ $(LDLIBS)

clean:
	-$(rm) *.o *~ midisds
