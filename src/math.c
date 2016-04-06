#include "math.h"
#include "vector.h"
#include <math.h>

#define abs(x) (((x) < 0) ? -(x) : (x))

void math_cross3f(vec3f_t u, vec3f_t v, vec3f_t result) {
    result[0] = u[1] * v[2] - u[2] * v[1];
    result[1] = u[2] * v[0] - u[0] * v[2];
    result[2] = u[0] * v[1] - u[1] * v[0];
}

void math_barycentric3f(vec3f_t a, vec3f_t b, vec3f_t c, vec3f_t p, vec3f_t result) {
    vec3f_t x;
    vec3f_t y;
    vec3f_t cross;

    x[0] = b[0] - a[0];
    x[1] = c[0] - a[0];
    x[2] = a[0] - p[0];

    y[0] = b[1] - a[1];
    y[1] = c[1] - a[1];
    y[2] = a[1] - p[1];

    math_cross3f(x, y, cross);

    if (abs(cross[2]) < 1.0f) {
        result[0] = -1.0f;
        result[1] = 1.0f;
        result[2] = 1.0f;
        return;
    }
    result[0] = 1.0f - (cross[0] + cross[1]) / cross[2];
    result[1] = cross[1] / cross[2];
    result[2] = cross[0] / cross[2];
}

void math_normal(vec3f_t a, vec3f_t b, vec3f_t c, vec3f_t result) {
    vec3f_t x;
    vec3f_t y;

    x[0] = c[0] - a[0];
    x[1] = c[1] - a[1];
    x[2] = c[2] - a[2];

    y[0] = b[0] - a[0];
    y[1] = b[1] - a[1];
    y[2] = b[2] - a[2];

    math_cross3f(x, y, result);
}

void math_normalize(vec3f_t v) {
    float n = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= n;
    v[1] /= n;
    v[2] /= n;
}

float math_dotproduct(vec3f_t u, vec3f_t v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}
