#include <stdio.h>

#include "framebuffer.h"
#include "SDL.h"
#include "vertex.h"

#define rgb_to_565(r, g, b) ((((r) & 0x1F) << 11) | (((g) & 0x3F) << 5) | ((b) & 0x1F))

int main (int argc, char **argv) {
    int error;
    framebuffer_t fb;

    error = framebuffer_init(&fb, 640, 480);

    if (error != 0) {
        printf("Error code: %d\n", error);
    }

    vertex2i_t p1 = {200, 100};
    vertex2i_t p2 = {300, 300};
    vertex2i_t p3 = {100, 300};

    framebuffer_trianglef(&fb, p1, p2, p3, rgb_to_565(0x00, 0xFF, 0x00));
    framebuffer_triangle(&fb, p1, p2, p3, rgb_to_565(0xFF, 0xFF, 0xFF));

    framebuffer_flip(&fb);

    SDL_Delay(2000);

    framebuffer_quit(&fb);
}
