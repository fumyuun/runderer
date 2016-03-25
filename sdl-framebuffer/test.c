#include <stdio.h>

#include "framebuffer.h"
#include "SDL.h"

int main (int argc, char **argv) {
    int error;
    framebuffer_t fb;

    error = framebuffer_init(&fb, 640, 480);

    if (error != 0) {
        printf("Error code: %d\n", error);
    }

    framebuffer_rect(&fb, 100, 100, 100, 100, 0x0000FF);
    framebuffer_rect(&fb, 300, 300, 100, 100, 0x00F000);

    framebuffer_flip(&fb);

    SDL_Delay(2000);

    framebuffer_quit(&fb);
}