#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "rasterizer.h"
#include "vector.h"
#include "math.h"
#include "glapi.h"

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
    vec4f_t bc;      // the barycentric cordinate of p with respect to triangle p1p2p3
    vec3f_t normal;  // the face normal of triangle p1p2p3
    float intensity; // the light intensity
    bbox_t bbox;     // the bounding box of our triangle

    // Check if we need to cull this face
    if (self->active_mode & GL_CULL_FACE) {
        if (self->active_mode & GL_FRONT_AND_BACK) {
            // Front and back culling enabled, bye bye
            *frag_buf_end = frag_buf_begin;
            return;
        }
        // compute and normalize our face normal
        math_normal(p1.position, p2.position, p3.position, normal);
        math_normalize(normal);

        // the light intensity
        intensity = math_dotproduct(normal, self->eye_direction);

        // our light intensity is negative if we have a back face
        if ((intensity < 0.0f && (self->active_mode & GL_BACK)) ||
            (intensity >= 0.0f && (self->active_mode & GL_FRONT))) {
            *frag_buf_end = frag_buf_begin;
            return;
        }
    }

    fragment_t* frag_buf_current = frag_buf_begin;

    get_bbox(p1.position, p2.position, p3.position, &bbox);

    if (bbox.xmin < 0) bbox.xmin = 0;
    if (bbox.ymin < 0) bbox.ymin = 0;
    if (bbox.xmax > (int)self->framebuffer->width)  bbox.xmax = self->framebuffer->width;
    if (bbox.ymax > (int)self->framebuffer->height) bbox.ymax = self->framebuffer->height;

    // iterate over all points to consider if they're in the triangle
    for (int y = bbox.ymin; y < bbox.ymax; ++y) {
        for (int x = bbox.xmin; x < bbox.xmax; ++x) {
            fragment_t frag = {
                .screen = {x, y, 0},
                .color = {0.0f, 0.0f, 0.0f, 1.0f}
            };

            // if any component of the bc coordinates is negative, the point is not inside the triangle
            math_barycentric3f_denorm(p1.position, p2.position, p3.position, frag.screen, bc);

            if (bc[0] >= 0.0f && bc[1] >= 0.0f && bc[2] >= 0.0f) {
                assert(frag_buf_current < *frag_buf_end);
                frag.screen[2] = (p1.position[2] * bc[0] + p2.position[2] * bc[1] + p3.position[2] * bc[2]) / bc[3];
                for(unsigned int i = 0; i < 4; ++i){
                    frag.color[i] = (p1.color[i] * bc[0] + p2.color[i] * bc[1] + p3.color[i] * bc[2]) / bc[3];
                }
                *frag_buf_current++ = frag;
            }
        }
    }
    *frag_buf_end = frag_buf_current;
}

void rasterize_triangle_edge(struct runderer* self, stream_t p0, stream_t p1, stream_t p2, fragment_t* frag_buf_begin, fragment_t** frag_buf_end) {
    vec3f_t bc;      // the barycentric cordinate of p with respect to triangle p0p1p2
    vec3f_t normal;  // the face normal of triangle p0p1p2
    float intensity; // the light intensity
    bbox_t bbox;     // the bounding box of our triangle

    // Check if we need to cull this face
    if (self->active_mode & GL_CULL_FACE) {
        if (self->active_mode & GL_FRONT_AND_BACK) {
            // Front and back culling enabled, bye bye
            *frag_buf_end = frag_buf_begin;
            return;
        }
        // compute and normalize our face normal
        math_normal(p0.position, p1.position, p2.position, normal);
        math_normalize(normal);

        // the light intensity
        intensity = math_dotproduct(normal, self->eye_direction);

        // our light intensity is negative if we have a back face
        if ((intensity < 0.0f && (self->active_mode & GL_BACK)) ||
            (intensity >= 0.0f && (self->active_mode & GL_FRONT))) {
            *frag_buf_end = frag_buf_begin;
            return;
        }
    }
    // sort over y axis
    stream_t ptemp;
    if (p0.position[1] > p1.position[1]) {
        ptemp = p0;
        p0 = p1;
        p1 = ptemp;
    }
    if (p0.position[1] > p2.position[1]) {
        ptemp = p0;
        p0 = p2;
        p2 = ptemp;
    }
    if (p1.position[1] > p2.position[1]) {
        ptemp = p1;
        p1 = p2;
        p2 = ptemp;
    }

    get_bbox(p0.position, p1.position, p2.position, &bbox);

    if (bbox.xmin < 0) bbox.xmin = 0;
    if (bbox.ymin < 0) bbox.ymin = 0;
    if (bbox.xmax > (int)self->framebuffer->width)  bbox.xmax = self->framebuffer->width;
    if (bbox.ymax > (int)self->framebuffer->height) bbox.ymax = self->framebuffer->height;

    float dy_1 = p1.position[1] - p0.position[1];
    float dx_1 = p1.position[0] - p0.position[0];
    float dy_2 = p2.position[1] - p1.position[1];
    float dx_2 = p2.position[0] - p1.position[0];
    float dy_3 = p0.position[1] - p2.position[1];
    float dx_3 = p0.position[0] - p2.position[0];

    float   e_1 = (bbox.xmin - p1.position[0]) * dy_1 - (bbox.ymin - p1.position[1]) * dx_1,
            e_2 = (bbox.xmin - p2.position[0]) * dy_2 - (bbox.ymin - p2.position[1]) * dx_2,
            e_3 = (bbox.xmin - p0.position[0]) * dy_3 - (bbox.ymin - p0.position[1]) * dx_3;

    fragment_t* frag_buf_current = frag_buf_begin;

    int x = bbox.xmin, y = bbox.ymin;

    int step_x = 1;
    while (y < bbox.ymax) {
        fragment_t frag = {
            .screen = {x, y, 0},
            .color = {
                0.0f,
                0.0f,
                1.0f,
                1.0f}
        };
        if (dx_2 >= 0 && e_1 >= 0 && e_2 >= 0 && e_3 >= 0) {
            *frag_buf_current++ = frag;
        }

        if (dx_2 <= 0 && e_1 <= 0 && e_2 <= 0 && e_3 <= 0) {
            *frag_buf_current++ = frag;
        }
        if ((step_x > 0 && x >= bbox.xmax) || (step_x < 0 && x <= bbox.xmin)) {
            // Ei(x, y+1)
            ++y;
            step_x *= -1;
            e_1 -= dx_1;
            e_2 -= dx_2;
            e_3 -= dx_3;
        }
        e_1 += step_x * dy_1;
        e_2 += step_x * dy_2;
        e_3 += step_x * dy_3;
        x += step_x;
    }

    *frag_buf_end = frag_buf_current;
}
