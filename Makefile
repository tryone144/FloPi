#
# FloPi
#
# Floppy Music for Raspberry Pi
#
# (c) 2015 Bernd Busse
#

VERSION = 0.1.0
CC      = /usr/bin/gcc
CFLAGS  = -Wall -g -DVERSION=\"$(VERSION)\"
LDFLAGS = 

OBJ = src/flopi-driver.o src/flopi-gpio.o src/flopi-midi.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o bin/flopi $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

#OBJ = datei1.o datei2.o datei3.o datei4.o datei5.o
#
#prog: $(OBJ)
#	        $(CC) $(CFLAGS) -o prog $(OBJ) $(LDFLAGS)
#
#%.o: %.c
#	        $(CC) $(CFLAGS) -c $<
