#include <assert.h>
#include "rasterizer.h"
#include "vector.h"
#include "math.h"

void rasterize_triangle(struct runderer* self, stream_t p1, stream_t p2, stream_t p3, fragment_t* frag_buf_begin, fragment_t** frag_buf_end) {
    vec3f_t bc;      // the barycentric cordinate of p with respect to triangle p1p2p3
    vec3f_t normal;  // the face normal of triangle p1p2p3
    float intensity;    // the light intensity

    // compute and normalize our face normal
    math_normal(p1.position, p2.position, p3.position, normal);
    math_normalize(normal);

    // the light intensity
    intensity = math_dotproduct(normal, self->eye_direction);

    // our light intensity is negative if we have a back face
    if (intensity < 0.0f) {
        printf("Culled back-face\n");
        return;
    }

    fragment_t* frag_buf_current = frag_buf_begin;

    // iterate over all points to consider if they're in the triangle
    for (uint y = 0; y < self->framebuffer->height; ++y) {
        for (uint x = 0; x < self->framebuffer->width; ++x) {
            fragment_t frag = {
                .screen = {x, y, 0},
                .color = {intensity * p1.color[0], intensity * p1.color[1], intensity * p1.color[2], 1.0f}
            };

            // if any component of the bc coordinates is negative, the point is not inside the triangle
            math_barycentric3f(p1.position, p2.position, p3.position, frag.screen, bc);
            if (bc[0] >= 0.0f && bc[1] >= 0.0f && bc[2] >= 0.0f) {
                assert(frag_buf_current < *frag_buf_end);
                frag.screen[2] = p1.position[2] * bc[0] + p2.position[2] * bc[1] + p3.position[2] * bc[2];
                *frag_buf_current++ = frag;
            }
        }
    }
    *frag_buf_end = frag_buf_current;
}
