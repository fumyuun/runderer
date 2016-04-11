#include <math.h>
#include <stdio.h>
#include <string.h>
#include "glapi.h"
#include "runderer.h"

// the static main instance of our runderer
static runderer_t *runderer = NULL;

void runderer_activate(runderer_t *run) {
    runderer = run;
}

void glMatrixMode(int mode) {
    switch(mode) {
        case GL_MODELVIEW:  runderer->active_matrix = &runderer->model_matrix;        break;
        case GL_PROJECTION: runderer->active_matrix = &runderer->projection_matrix;   break;
        default:
            printf("Invalid glMatrixMode\n");
    }
}

void glViewport(int x, int y, int w, int h) {
    mat_identity(runderer->viewport_matrix);
    const float depth = 10.0f; // temporary value for now
    runderer->viewport_matrix[0        ] = w / 2.0f;
    runderer->viewport_matrix[4 * 0 + 3] = x + w / 2.0f;
    runderer->viewport_matrix[4 * 1 + 1] = h / 2.0f;
    runderer->viewport_matrix[4 * 1 + 3] = y + h / 2.0f;
    runderer->viewport_matrix[4 * 2 + 2] = depth / 2.0f;
    runderer->viewport_matrix[4 * 2 + 3] = depth / 2.0f;
}

void glRotatef(float angle, float x, float y, float z) {
    mat4f_t rotated;
    const float c = cosf(angle);
    const float s = sinf(angle);
    const mat4f_t rot = {
          x*x*(1-c)+c,  y*x*(1-c)+z*s,  x*z*(1-c)-y*s,  0,
        x*y*(1-c)-z*s,    y*y*(1-c)+c,  y*z*(1-c)+x*s,  0,
        x*z*(1-c)+y*s,  y*z*(1-c)-x*s,    z*z*(1-c)+c,  0,
                    0,              0,              0,  1
    };

    mat_mult_mat4f(runderer->model_matrix, rot, rotated);
    memcpy(runderer->model_matrix, rotated, 4 * 4 * sizeof(float));
}
