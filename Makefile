#
# FloPi
#
# Floppy Music for Raspberry Pi
#
# (c) 2015 Bernd Busse
#

include ./Makefile.rules

$(OBJ) = src/*.o

all:
	make -c src/
	$(CC) $(CCFLAGS) -o /bin/flopi $(OBJ) $(LDFLAGS)

clean:
	rm -rf bin/
	make -c src/ clean

