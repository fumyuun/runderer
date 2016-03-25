#include <stdio.h>
#include <unistd.h>

#include "framebuffer.h"

int main (int argc, char **argv) {
    int error;
    framebuffer_t fb;

    error = framebuffer_init(&fb, 640, 480);

    if (error != 0) {
        printf("Error: %d\n", error);
    }

    framebuffer_rect(&fb, 100, 100, 100, 100, 0x0000FF);
    framebuffer_rect(&fb, 300, 300, 100, 100, 0x00F000);

    framebuffer_flip(&fb);

    sleep(2);

    framebuffer_quit(&fb);
}