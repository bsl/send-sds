rm = rm -rf
binaries := midisds
binaries := $(addprefix "src/", $(binaries))

.PHONY: clean all bin lib

all: bin lib
	$(MAKE) -C src && \
	mv $(binaries) bin

bin:
	mkdir bin

lib:
	mkdir lib

clean:
	-$(rm) bin
	$(MAKE) -C src clean