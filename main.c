#include <stdio.h>

#include "framebuffer.h"
#include "runderer.h"
#include "vector.h"
#include "matrix.h"
#include "src/rasterizer.h"

#include <SDL2/SDL.h>

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

    vertex_t vert[6] = {
        {
            .position = {200.0f, 100.0f, 10.0f, 1.0f},
            .color    = {1.0f, 0.0f, 0.0f, 1.0f}
        },{
            .position = {300.0f, 300.0f, 10.0f, 1.0f},
            .color    = {0.0f, 1.0f, 0.0f, 1.0f}
        },{
            .position = {100.0f, 300.0f, 0.0f, 1.0f},
            .color    = {0.0f, 0.0f, 1.0f, 1.0f}
        },{
            .position = {250.0f, 150.0f, 0.0f, 1.0f},
            .color    = {0.0f, 1.0f, 1.0f, 1.0f}
        },{
            .position = {350.0f, 350.0f, 100.0f, 1.0f},
            .color    = {0.0f, 1.0f, 1.0f, 1.0f}
        },{
            .position = {150.0f, 350.0f, 100.0f, 1.0f},
            .color    = {1.0f, 1.0f, 0.0f, 1.0f}
        }
    };

    mat_identity(run.model_matrix);
    mat_identity(run.view_matrix);
    mat_identity(run.projection_matrix);
    mat_identity(run.viewport_matrix);

    run.draw_triangle_array = runderer_draw_triangle_array;
    run.vertex_shader = runderer_vertex_shader;
    run.triangle_rasterizer = rasterize_triangle;
    run.fragment_shader = runderer_fragment_shader_flat;

    runderer_draw_triangle_array(&run, vert, 2);

    framebuffer_flip(&fb);

    SDL_Delay(2000);

    framebuffer_quit(&fb);

    return 0;
}
