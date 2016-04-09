#include "matrix.h"
#include "vector.h"
#include <string.h>

void mat_identity(mat4f_t m) {
    static const mat4f_t i = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    memcpy(m, i, sizeof(mat4f_t));
}

void mat_mult_vec4f(const mat4f_t m, const vec4f_t v, vec4f_t res) {
    for (int y = 0; y < 4; ++y) {
        res[y] = 0;
        for (int x = 0; x < 4; ++x) {
            res[y] += m[4 * y + x] * v[x];
        }
    }
}

void mat_mult_mat4f(const mat4f_t m, const mat4f_t w, mat4f_t res) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            res[y * 4 + x] = 0;
            for (int i = 0; i < 4; ++i) {
                res[y * 4 + x] += m[y * 4 + i] * w[i * 4 + x];
            }
        }
    }
}
