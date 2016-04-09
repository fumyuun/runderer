CFLAGS=$(shell sdl2-config --cflags) -Iinclude -g
LDFLAGS=$(shell sdl2-config --libs) -lm

main:	main.o framebuffer.o math.o runderer.o matrix.o rasterizer.o glapi.o

%.o:	src/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
