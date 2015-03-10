#
# FloPi
#
# Floppy Music for Raspberry Pi
#
# (c) 2015 Bernd Busse
#

include ./Makefile.rules

SRCDIRS = src
BINDIRS = bin
BUILDSRC = $(SRCDIRS:%=build-%)
CLEANSRC = $(SRCDIRS:%=clean-%)
CLEANBIN = $(BINDIRS:%=clean-%)

all: $(BUILDSRC) flopi
$(SRCDIRS): $(BUILDSRC)
$(BUILDSRC):
	$(MAKE) -C $(@:build-%=%)

flopi: $(SRCDIRS)
	mkdir -p $(BINDIRS)
	$(CC) $(CCFLAGS) -o bin/flopi $(SRCDIRS:%=%/*.o) $(LDFLAGS)

clean: $(CLEANSRC) $(CLEANBIN)
$(CLEANSRC):
	$(MAKE) -C $(@:clean-%=%) clean
$(CLEANBIN):
	rm -rf $(@:clean-%=%)

.PHONY: subdirs $(SRCDIRS) $(BINDIRS)
.PHONY: subdirs $(BUILDSRC)
.PHONY: subdirs $(CLEANSRC) $(CLEANBIN)
.PHONY: all clean flopi

