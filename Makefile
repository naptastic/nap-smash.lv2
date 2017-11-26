CFLAGS = -Wall -I/usr/local/include

all: nap-smash.so

nap-smash.so: nap-smash.o
	$(LD) nap-smash.o -o nap-smash.so -shared

clean:
	rm -f *.o nap-smash.so
