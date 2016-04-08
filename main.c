#include <stdio.h>
#include <math.h>

#include "framebuffer.h"
#include "runderer.h"
#include "vector.h"
#include "matrix.h"
#include "src/rasterizer.h"

#include <SDL2/SDL.h>

void glRotatef(mat4f_t mat, float angle, float x, float y, float z, mat4f_t result);

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
        }/*,{
            .position = {250.0f, 150.0f, 0.0f, 1.0f},
            .color    = {1.0f, 0.0f, 1.0f, 1.0f}
        },{
            .position = {350.0f, 350.0f, 100.0f, 1.0f},
            .color    = {0.0f, 1.0f, 1.0f, 1.0f}
        },{
            .position = {150.0f, 350.0f, 100.0f, 1.0f},
            .color    = {1.0f, 1.0f, 0.0f, 1.0f}
        }*/
    };

    mat_identity(run.model_matrix);
    mat_identity(run.view_matrix);
    mat_identity(run.projection_matrix);
    mat_identity(run.viewport_matrix);

    run.draw_triangle_array = runderer_draw_triangle_array;
    run.vertex_shader = runderer_vertex_shader;
    run.triangle_rasterizer = rasterize_triangle;
    run.fragment_shader = runderer_fragment_shader_flat;

    float depth = 10.0f; // temporary value for now
    float x = 0, y = 0, w = 640, h = 480;
    run.viewport_matrix[0        ] = w / 2.0f;
    run.viewport_matrix[4 * 0 + 3] = x + w / 2.0f;
    run.viewport_matrix[4 * 1 + 1] = h / 2.0f;
    run.viewport_matrix[4 * 1 + 3] = y + h / 2.0f;
    run.viewport_matrix[4 * 2 + 2] = depth / 2.0f;
    run.viewport_matrix[4 * 2 + 3] = depth / 2.0f;

    mat4f_t rotated;

    for (int i = 0; i < 10; ++i) {
        framebuffer_clear(&fb);
        mat_identity(run.model_matrix);
        glRotatef(run.model_matrix, 0.1f * M_PI * i, 0.0f, 0.0f, 1.0f, rotated);
        memcpy(run.model_matrix, rotated, 4 * 4 * sizeof(float));

        runderer_draw_triangle_array(&run, vert, 1);

        framebuffer_flip(&fb);
        SDL_Delay(500);
    }

    framebuffer_quit(&fb);

    return 0;
}

void glRotatef(mat4f_t mat, float angle, float x, float y, float z, mat4f_t result) {
    angle = angle;
    float c = cos(angle);
    float s = sin(angle);
    mat4f_t rot = {
          x*x*(1-c)+c,  y*x*(1-c)+z*s,  x*z*(1-c)-y*s,  0,
        x*y*(1-c)-z*s,    y*y*(1-c)+c,  y*z*(1-c)+x*s,  0,
        x*z*(1-c)+y*s,  y*z*(1-c)-x*s,    z*z*(1-c)+c,  0,
                    0,              0,              0,  1
    };

    mat_mult_mat4f(mat, rot, result);
}
