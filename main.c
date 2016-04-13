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


#define MIN_MACRO(x, y) ((x) < (y) ? (x) : (y))

float min_f(float const x, float const y){
    return MIN_MACRO(x, y);
}

float min_i(int const x, int const y){
    return MIN_MACRO(x, y);
}

#undef MIN_MACRO

#define M_PIF 3.1415926536f

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

    vertex_t pyramid_head[] = {
        {
            .position = {-0.5f, 0.0f, -0.5f, 1.0f},
            .color    = {1.0f, 0.0f, 0.0f, 1.0f}
        },{
            .position = {0.0f, -0.5f, 0.0f, 1.0f},
            .color    = {1.0f, 0.0f, 0.0f, 1.0f}
        },{
            .position = {0.5f, 0.0f, -0.5f, 1.0f},
            .color    = {1.0f, 0.0f, 0.0f, 1.0f}
        },

        {
            .position = {0.5f, 0.0f, -0.5f, 1.0f},
            .color    = {0.0f, 1.0f, 0.0f, 1.0f}
        },{
            .position = {0.0f, -0.5f, 0.0f, 1.0f},
            .color    = {0.0f, 1.0f, 0.0f, 1.0f}
        },{
            .position = {0.5f, 0.0f, 0.5f, 1.0f},
            .color    = {0.0f, 1.0f, 0.0f, 1.0f}
        },

        {
            .position = {0.5f, 0.0f, 0.5f, 1.0f},
            .color    = {0.0f, 0.0f, 1.0f, 1.0f}
        },{
            .position = {0.0f, -0.5f, 0.0f, 1.0f},
            .color    = {0.0f, 0.0f, 1.0f, 1.0f}
        },{
            .position = {-0.5f, 0.0f, 0.5f, 1.0f},
            .color    = {0.0f, 0.0f, 1.0f, 1.0f}
        },

        {
            .position = {-0.5f, 0.0f, 0.5f, 1.0f},
            .color    = {1.0f, 0.0f, 1.0f, 1.0f}
        },{
            .position = {0.0f, -0.5f, 0.0f, 1.0f},
            .color    = {1.0f, 0.0f, 1.0f, 1.0f}
        },{
            .position = {-0.5f, 0.0f, -0.5f, 1.0f},
            .color    = {1.0f, 0.0f, 1.0f, 1.0f}
        },
    };

    vertex_t pyramid_ass[] = {

        {.position = {-.5f, 0.0f, -.5f, 1.0f},
         .color    = {1.0f, 0.0f, 0.0f, 1.0f}},

        {.position = {0.5f, 0.0f, -.5f, 1.0f},
         .color    = {0.0f, 1.0f, 0.0f, 1.0f}},

        {.position = {0.5f, 0.0f, 0.5f, 1.0f},
         .color    = {0.0f, 0.0f, 1.0f, 1.0f}},

        {.position = {-.5f, 0.0f, 0.5f, 1.0f},
         .color    = {1.0f, 1.0f, 1.0f, 1.0f}},
    };

    mat_identity(run.model_matrix);
    mat_identity(run.view_matrix);
    mat_identity(run.projection_matrix);
    mat_identity(run.viewport_matrix);

    run.draw_triangle_array = runderer_draw_triangle_array;
    run.draw_quad_array = runderer_draw_quad_array;
    run.vertex_shader = runderer_vertex_shader;
    run.triangle_rasterizer = rasterize_triangle;
    run.fragment_shader = runderer_fragment_shader_flat;

    runderer_activate(&run);
    glViewport(0, 0, SCREEN_W, SCREEN_H);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    int quit = 0;
    SDL_Event event;
    uint32_t now = SDL_GetTicks();
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: quit = 1; break;
                default: break;
            }
        }


        float const dt = min_f((SDL_GetTicks() - now) / 1000.0f, 0.5f);
        now = SDL_GetTicks();
        float const current_time = now / 1000.0f;

        glRotatef(dt * sinf(current_time) * M_PIF, 0.0f, 1.0f, 0.0f);
        glRotatef(dt * cosf(current_time) * M_PIF, 1, 0, 0);

        framebuffer_clear(&fb);

        runderer_draw_triangle_array(&run, pyramid_head, 4);
        runderer_draw_quad_array(&run, pyramid_ass, 1);

        framebuffer_flip(&fb);
    }
    runderer_unbind(&run);

    framebuffer_quit(&fb);

    return 0;
}
