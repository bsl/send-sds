CFLAGS = -std=c99 -MMD -pedantic -W -Wall -Wno-variadic-macros -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winline -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-compare -Wstrict-prototypes -Wundef -Wwrite-strings
LDFLAGS =
LDLIBS = -lasound -lsndfile

exes    = send-sds receive-sds
deps    = $(addsuffix .d,$(basename $(wildcard *.c)))
libobjs = common.o err.o midi.o sds.o
libfile = libsds.a

dockertag = bsorahan/send-sds

rm = rm -f

all: $(exes)

$(libfile): $(libobjs)
	$(AR) crs $@ $^

$(exes): $(libfile)
	$(CC) $(CFLAGS) -o $@ $@.c $(libfile) $(LDFLAGS) $(LDLIBS)

clean:
	-$(rm) $(exes) $(deps) $(libfile) $(libobjs) *.log *~

image:
	docker build -t $(dockertag) .

.PHONY: all clean image

-include $(deps)
