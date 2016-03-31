CFLAGS=$(shell sdl2-config --cflags) -I../include
LDFLAGS=$(shell sdl2-config --libs)

test:	test.c framebuffer.o math.o

framebuffer.o: framebuffer.c framebuffer.h

math.o: ../math/math.c
	$(CC) -c -o $@ $^ $(CFLAGS)