CFLAGS=-I/opt/X11/include -g
LDFLAGS=-L/opt/X11/lib/ -lX11

test:	test.c framebuffer.o

framebuffer.o: framebuffer.c framebuffer.h

clean:
	rm -f test framebuffer.o