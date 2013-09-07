CC = gcc
CFLAGS = -std=c99 -pedantic -W -Wall -Wno-variadic-macros -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winline -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-compare -Wstrict-prototypes -Wundef -Wwrite-strings

LDLIBS = -lasound

rm = rm -f

all: send-sds receive-sds dump-sds

send-sds: send-sds.o err.o midi.o sds.o
receive-sds: receive-sds.o err.o midi.o sds.o
dump-sds: dump-sds.o err.o midi.o sds.o

.PHONY: clean

clean:
	-$(rm) *.o
	-$(rm) send-sds
	-$(rm) receive-sds
	-$(rm) dump-sds
	-$(rm) *~