CFLAGS=$(shell sdl2-config --cflags) -Iinclude -g -Wall -Wextra -pedantic -DFB_SDL
LDFLAGS=$(shell sdl2-config --libs) -lm

main:	main.o framebuffer_sdl.o math.o runderer.o matrix.o rasterizer.o glapi.o

%.o:	src/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

%.o:	src/framebuffer/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
