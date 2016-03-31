CFLAGS=$(shell sdl2-config --cflags) -Iinclude
LDFLAGS=$(shell sdl2-config --libs)

main:	main.c framebuffer.o math.o runderer.o

%.o:	src/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)