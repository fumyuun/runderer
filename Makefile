CFLAGS=$(shell sdl2-config --cflags) -Iinclude -g -Wall -Wextra -pedantic
LDFLAGS=$(shell sdl2-config --libs) -lm

main:	main.o runderer.a framebuffer_sdl.o

runderer.a: math.o runderer.o matrix.o rasterizer.o glapi.o
	ar rcs $@ $^

%.o:	src/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

%.o:	src/framebuffer/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
