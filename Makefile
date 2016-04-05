CFLAGS=$(shell sdl2-config --cflags) -Iinclude
LDFLAGS=$(shell sdl2-config --libs) -lm

main:	main.c framebuffer.o math.o runderer.o matrix.o rasterizer.o

%.o:	src/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
