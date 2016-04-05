#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef float mat3f_t [3*3];
typedef float mat4f_t [4*4];

void mat_identity(mat4f_t m);

void mat_mult_vec4f(mat4f_t m, vec4f_t v, mat4f_t res);

void mat_mult_mat4f(mat4f_t m, mat4f_t w, mat4f_t res);

#endif /* MATRIX_H */
