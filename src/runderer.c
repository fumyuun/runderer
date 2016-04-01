#include "runderer.h"
#include "math.h"
#include <stdint.h>
#include <stdlib.h>

// Convert r, g, b (in bytes) to a 16-bit 5R6G5B color
#define rgb_to_565(r, g, b) (uint16_t)((((r) & 0x1F) << 11) | (((g) & 0x3F) << 5) | ((b) & 0x1F))

// Compute a bounding box for a triangle
void triangle_bbox(vec3f_t p1, vec3f_t p2, vec3f_t p3, vec2i_t min, vec2i_t max) {
    float *points[3] = {p1, p2, p3};
    min[0] = max[0] = points[0][0];
    min[1] = max[1] = points[0][1];
    for (int i = 1; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (points[i][j] < min[j]) min[j] = points[i][j];
            if (points[i][j] > max[j]) max[j] = points[i][j];
        }
    }
}

/**
 * Bind the runderer instance to a framebuffer driver
 * \param run a pointer to the runderer instance to bind
 * \param fb a pointer to the framebuffer instance to bind
 * \return 0 if succesful, nonzero otherwise
 */
int runderer_bind(runderer_t *run, framebuffer_t *fb) {
    if (run == NULL || fb == NULL) {
        return 1;
    }

    run->framebuffer = fb;
    run->zbuffer = malloc(fb->width * fb->height * sizeof(ZBUF_TYPE));
    if (run->zbuffer == NULL) {
        return 2;
    }

    // For now, the light is just coming from the z-axis
    run->light_direction[0] = 0.0f;
    run->light_direction[1] = 0.0f;
    run->light_direction[2] = -1.0f;

    return 0;
}

/**
 * \brief Draw a triangle between three three-dimensional points
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param p3 the third point
 * \param color the color of the triangle, in 5R6G5B color format
 */
void runderer_triangle(runderer_t *run, vec3f_t p1, vec3f_t p2, vec3f_t p3, unsigned int color) {
    printf("Stub called :(\n");
}

/**
 * \brief Draw a filled triangle between three three-dimensional points using
 * a simple flat shaded color.
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param p3 the third point
 * \param color the color of the triangle
 */
void runderer_trianglef(runderer_t *run, vec3f_t p1, vec3f_t p2, vec3f_t p3, vec3f_t color) {
    vec3f_t p;       // the current point to consider rasterizing
    vec3f_t bc;      // the barycentric cordinate of p with respect to triangle p1p2p3
    vec3f_t normal;  // the face normal of triangle p1p2p3
    vec2i_t bb_min;  // bounding box of the triangle
    vec2i_t bb_max;
    float intensity;    // the light intensity
    unsigned int color_shaded;  // the shaded color, linearly interpolated over intensity (for now)

    // compute and normalize our face normal
    math_normal(p1, p2, p3, normal);
    math_normalize(normal);

    // the light intensity
    intensity = math_dotproduct(normal, run->light_direction);

    // our light intensity is negative if we have a back face
    if (intensity < 0.0f) {
        printf("Culled back-face\n");
        return;
    }

    // lets just interpolate over the color for now
    color_shaded = rgb_to_565((unsigned int)(0xFF * intensity * color[0]),
                              (unsigned int)(0xFF * intensity * color[1]),
                              (unsigned int)(0xFF * intensity * color[2]));

    // get the bounding box of the triangle
    triangle_bbox(p1, p2, p3, bb_min, bb_max);

    // clip the bounding box to the screen
    if (bb_min[0] < 0) bb_min[0] = 0;
    if (bb_min[1] < 0) bb_min[1] = 0;
    if (bb_max[0] > run->framebuffer->width)  bb_max[0] = run->framebuffer->width;
    if (bb_max[1] > run->framebuffer->height) bb_max[1] = run->framebuffer->height;

    // iterate over all points to consider if they're in the triangle
    for (int y = bb_min[1]; y < bb_max[1]; ++y) {
        for (int x = bb_min[0]; x < bb_max[0]; ++x) {
            p[0] = x;
            p[1] = y;
            p[2] = 0;
            // if any component of the bc coordinates is negative, the point is not inside the triangle
            math_barycentric3f(p1, p2, p3, p, bc);
            if (bc[0] >= 0.0f && bc[1] >= 0.0f && bc[2] >= 0.0f) {
                p[2] = p1[2] * bc[0] + p2[2] * bc[1] + p3[2] * bc[2];
                // calculate and update it's z-coordinate and do a z-buffer test
                if (run->zbuffer[y * run->framebuffer->width + x] < p[2]) {
                    run->zbuffer[y * run->framebuffer->width + x] = p[2];
                    run->framebuffer->buf16[y * run->framebuffer->width + x] = color_shaded;
                }
            }
        }
    }
}
