#include <stdio.h>

#include "framebuffer.h"
#include "runderer.h"
#include "SDL.h"
#include "vertex.h"

#define rgb_to_565(r, g, b) ((((r) & 0x1F) << 11) | (((g) & 0x3F) << 5) | ((b) & 0x1F))

int main (int argc, char **argv) {
    int error;
    framebuffer_t fb;
    runderer_t run;

    error = framebuffer_init(&fb, 640, 480);

    if (error != 0) {
        printf("FB error code: %d\n", error);
        return 1;
    }

    error = runderer_bind(&run, &fb);

    if (error != 0) {
        printf("Runderer error code: %d\n", error);
        return 2;
    }

    vertex3f_t t1_p1 = {200.0f, 100.0f, 10.0f};
    vertex3f_t t1_p2 = {300.0f, 300.0f, 10.0f};
    vertex3f_t t1_p3 = {100.0f, 300.0f, 0.0f};
    vertex3f_t t1_col = {0.0f, 1.0f, 0.0f};

    vertex3f_t t2_p1 = {250.0f, 150.0f, 0.0f};
    vertex3f_t t2_p2 = {350.0f, 350.0f, 100.0f};
    vertex3f_t t2_p3 = {150.0f, 350.0f, 100.0f};
    vertex3f_t t2_col = {0.0f, 1.0f, 0.0f};

    runderer_triangle_flat(&run, t1_p1, t1_p2, t1_p3, t1_col);
    runderer_triangle_flat(&run, t2_p1, t2_p2, t2_p3, t2_col);

    framebuffer_flip(&fb);

    SDL_Delay(2000);

    framebuffer_quit(&fb);

    return 0;
}
