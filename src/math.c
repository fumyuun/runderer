#include "math.h"
#include "vertex.h"
#include <math.h>

#define abs(x) (((x) < 0) ? -(x) : (x))

void math_cross3f(vertex3f_t u, vertex3f_t v, vertex3f_t result) {
    result[0] = u[1] * v[2] - u[2] * v[1];
    result[1] = u[2] * v[0] - u[0] * v[2];
    result[2] = u[0] * v[1] - u[1] * v[0];
}

void math_barycentric(vertex2i_t a, vertex2i_t b, vertex2i_t c, vertex2i_t p, vertex3f_t result) {
    vertex3f_t x;
    vertex3f_t y;
    vertex3f_t cross;

    x[0] = b[0] - a[0];
    x[1] = c[0] - a[0];
    x[2] = a[0] - p[0];

    y[0] = b[1] - a[1];
    y[1] = c[1] - a[1];
    y[2] = a[1] - p[1];

    math_cross3f(x, y, cross);

    if (abs(cross[2]) < 1.0f) {
        result[0] = -1;
        result[1] = 1;
        result[2] = 1;
        return;
    }
    result[0] = 1.0f - (cross[0] + cross[1]) / cross[2];
    result[1] = cross[1] / cross[2];
    result[2] = cross[0] / cross[2];
}

void math_normal(vertex3f_t a, vertex3f_t b, vertex3f_t c, vertex3f_t result) {
    vertex3f_t x;
    vertex3f_t y;

    x[0] = c[0] - a[0];
    x[1] = c[1] - a[1];
    x[2] = c[2] - a[2];

    y[0] = b[0] - a[0];
    y[1] = b[1] - a[1];
    y[2] = b[2] - a[2];

    math_cross3f(x, y, result);
}

void math_normalize(vertex3f_t v) {
    float n = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= n;
    v[1] /= n;
    v[2] /= n;
}

float math_dotproduct(vertex3f_t u, vertex3f_t v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}