rm = rm -rf

ifeq ($(DESTDIR),)
	DESTDIR=/usr/local/
endif

.PHONY: clean all bin lib install

all: midisds

midisds:
	$(MAKE) -C src && mv src/midisds bin

clean:
	$(MAKE) -C src clean
	-$(rm) bin/midisds

install:
	cp bin/midisds $(DESTDIR)bin
	cp -R conf/midisds $(DESTDIR)etc
