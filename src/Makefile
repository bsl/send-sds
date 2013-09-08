CC = gcc
CFLAGS = -std=c99 -pedantic -W -Wall -Wno-variadic-macros -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winline -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-compare -Wstrict-prototypes -Wundef -Wwrite-strings

LDLIBS = $(pkg-config --cflags --libs asound sndfile)

rm = rm -f

.PHONY: clean all

all: midisds

midisds: midisds_main.c
	$(CC) $(CFLAGS) -o $@ midisds_main.c $(LDLIBS)

clean:
	-$(rm) *.o *~ midisds