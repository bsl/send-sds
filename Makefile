compiler       = gcc
compiler_flags = -std=c99 -pedantic -W -Wall -Wno-variadic-macros -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winline -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-compare -Wstrict-prototypes -Wundef -Wwrite-strings

sources   = $(wildcard *.c)
headers   = $(wildcard *.h)
libraries = -lasound

rm = rm -f

all: send-sds receive-sds

send-sds: send-sds.c err.c midi.c sds.c $(headers)
	$(compiler) $(compiler_flags) -o $@ $^ $(libraries)

receive-sds: receive-sds.c err.c midi.c sds.c $(headers)
	$(compiler) $(compiler_flags) -o $@ $^ $(libraries)

.PHONY: clean

clean:
	-$(rm) send-sds
	-$(rm) receive-sds
	-$(rm) *~