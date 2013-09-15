rm = rm -rf
directories := bin lib
binaries := midisds
binaries := $(addprefix "src/", $(binaries))

.PHONY: clean all bin lib

all: dirs
	$(MAKE) -C src && mv $(binaries) bin

dirs:
	mkdir $(directories)

clean:
	-$(rm) $(directories)
	$(MAKE) -C src clean