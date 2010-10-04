compiler       = gcc
compiler_flags = -W -Wall -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winline -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-compare -Wstrict-prototypes -Wundef -Wwrite-strings

sources   = $(wildcard *.c)
headers   = $(wildcard *.h)
libraries = -lasound

rm = rm -f

all: send-sds

send-sds: $(sources) $(headers)
	$(compiler) $(compiler_flags) -o $@ $(sources) $(libraries)

.PHONY: clean
clean:
	-$(rm) send-sds
