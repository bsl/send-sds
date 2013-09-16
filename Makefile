rm = rm -rf
midisd_src=src/midisd
midisd_bin=bin/midisd

ifeq ($(DESTDIR),)
	DESTDIR=/usr/local/
endif

.PHONY: clean all midisd install

all: midisd

midisd:
	$(MAKE) -C src && mv $(midisd_src) bin

clean:
	$(MAKE) -C src clean
	-$(rm) $(midisd_bin)

install:
	cp $(midisd_bin) $(DESTDIR)bin
	cp -R conf/midisd $(DESTDIR)etc
