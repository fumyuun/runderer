#include <assert.h>
#include "rasterizer.h"
#include "vector.h"
#include "math.h"

typedef struct {
    uint xmin;
    uint ymin;
    uint xmax;
    uint ymax;
} bbox_t;

void get_bbox(vec3f_t p1, vec3f_t p2, vec3f_t p3, bbox_t *result) {
    result->xmin = result->xmax = p1[0];
    result->ymin = result->ymax = p1[1];

    if (result->xmin > p2[0]) result->xmin = p2[0];
    if (result->ymin > p2[1]) result->ymin = p2[1];
    if (result->xmax < p2[0]) result->xmax = p2[0];
    if (result->ymax < p2[1]) result->ymax = p2[1];
    if (result->xmin > p3[0]) result->xmin = p3[0];
    if (result->ymin > p3[1]) result->ymin = p3[1];
    if (result->xmax < p3[0]) result->xmax = p3[0];
    if (result->ymax < p3[1]) result->ymax = p3[1];

}

void rasterize_triangle(struct runderer* self, stream_t p1, stream_t p2, stream_t p3, fragment_t* frag_buf_begin, fragment_t** frag_buf_end) {
    vec3f_t bc;      // the barycentric cordinate of p with respect to triangle p1p2p3
    vec3f_t normal;  // the face normal of triangle p1p2p3
    float intensity; // the light intensity
    bbox_t bbox;     // the bounding box of our triangle

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

    get_bbox(p1.position, p2.position, p3.position, &bbox);

    // iterate over all points to consider if they're in the triangle
    for (uint y = bbox.ymin; y < bbox.ymax; ++y) {
        for (uint x = bbox.xmin; x < bbox.xmax; ++x) {
            fragment_t frag = {
                .screen = {x, y, 0},
                .color = {0.0f, 0.0f, 0.0f, 1.0f}
            };

            // if any component of the bc coordinates is negative, the point is not inside the triangle
            math_barycentric3f(p1.position, p2.position, p3.position, frag.screen, bc);
            assert(fabs(bc[0] + bc[1] + bc[2] - 1.0f) < 0.001f);

            if (bc[0] >= 0.0f && bc[1] >= 0.0f && bc[2] >= 0.0f) {
                assert(frag_buf_current < *frag_buf_end);
                frag.screen[2] = p1.position[2] * bc[0] + p2.position[2] * bc[1] + p3.position[2] * bc[2];
                for(uint i = 0; i < 4; ++i){
                    frag.color[i] = p1.color[i] * bc[0] + p2.color[i] * bc[1] + p3.color[i] * bc[2];
                }
                *frag_buf_current++ = frag;
            }
        }
    }
    *frag_buf_end = frag_buf_current;
}
