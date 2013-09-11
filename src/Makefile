CC = gcc
CFLAGS = -std=gnu99 -pedantic -W -Wall -Wno-variadic-macros -Wbad-function-cast -Wcast-align -Wcast-qual -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winline -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-compare -Wstrict-prototypes -Wundef -Wwrite-strings

LDLIBS := -lasound -lsndfile

rm = rm -f

.PHONY: clean all

all: midisds

midisds: midisds_command.o midisds_dump.o
	$(CC) $(CFLAGS) -o $@ midisds_main.c $^ $(LDLIBS)

clean:
	-$(rm) *.o *~ midisds
