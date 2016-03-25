CFLAGS=$(shell sdl2-config --cflags)
LDFLAGS=$(shell sdl2-config --libs)

test:	test.c framebuffer.o

framebuffer.o: framebuffer.c framebuffer.h