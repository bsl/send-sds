rm = rm -rf

binaries := midisds
binaries := $(addprefix "src/", $(binaries))

.PHONY: clean all bin

all: bin
	$(MAKE) -C src && mv $(binaries) bin

bin:
	mkdir bin

clean:
	-$(rm) bin
	$(MAKE) -C src clean