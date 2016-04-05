#include "matrix.h"
#include "vector.h"


void mat_identity(mat4f_t m) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            m[y * 4 + x] = (x == y ? 1.0f : 0.0f);
        }
    }
}

void mat_mult_vec4f(mat4f_t m, vec4f_t v, vec4f_t res) {
    for (int y = 0; y < 4; ++y) {
        res[y] = 0;
        for (int x = 0; x < 4; ++x) {
            res[y] += m[4 * y + x] * v[x];
        }
    }
}

void mat_mult_mat4f(mat4f_t m, mat4f_t w, mat4f_t res) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            res[y * 4 + x] = 0;
            for (int i = 0; i < 4; ++i) {
                res[y * 4 + x] += m[y * 4 + i] * w[i * 4 + x];
            }
        }
    }
}
