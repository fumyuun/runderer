#include <stdio.h>
#include <math.h>

#include "framebuffer.h"
#include "runderer.h"
#include "vector.h"
#include "matrix.h"
#include "glapi.h"
#include "src/rasterizer.h"

#define SCREEN_W 640
#define SCREEN_H 480

#include <SDL2/SDL.h>

int main (int argc, char **argv) {
    int error;
    framebuffer_t fb;
    runderer_t run;

    error = framebuffer_init(&fb, SCREEN_W, SCREEN_H);

    if (error != 0) {
        printf("FB error code: %d\n", error);
        return 1;
    }

    error = runderer_bind(&run, &fb);

    if (error != 0) {
        printf("Runderer error code: %d\n", error);
        return 2;
    }

    vertex_t vert[3] = {
        {
            .position = {0.0f, -1.0f, 0.0f, 1.0f},
            .color    = {1.0f, 0.0f, 0.0f, 1.0f}
        },{
            .position = {1.0f, 1.0f, 0.0f, 1.0f},
            .color    = {0.0f, 1.0f, 0.0f, 1.0f}
        },{
            .position = {-1.0f, 1.0f, 0.0f, 1.0f},
            .color    = {0.0f, 0.0f, 1.0f, 1.0f}
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

    runderer_activate(&run);
    glViewport(0, 0, SCREEN_W, SCREEN_H);

    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: quit = 1; break;
                default: break;
            }
        }

        framebuffer_clear(&fb);

        glRotatef(0.1f * M_PI, 0.0f, 0.0f, 1.0f);

        runderer_draw_triangle_array(&run, vert, 1);

        framebuffer_flip(&fb);
    }
    runderer_unbind(&run);

    framebuffer_quit(&fb);

    return 0;
}
