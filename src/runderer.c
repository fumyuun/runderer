#include "runderer.h"
#include "math.h"
#include <stdlib.h>

#define rgb_to_565(r, g, b) ((((r) & 0x1F) << 11) | (((g) & 0x3F) << 5) | ((b) & 0x1F))

int runderer_bind(runderer_t *run, framebuffer_t *fb) {
    if (run == NULL || fb == NULL) {
        return 1;
    }

    run->framebuffer = fb;
    run->zbuffer = malloc(fb->width * fb->height * sizeof(ZBUF_TYPE));
    if (run->zbuffer == NULL) {
        return 2;
    }

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
void runderer_triangle(runderer_t *run, vertex3f_t p1, vertex3f_t p2, vertex3f_t p3, unsigned int color) {

}

/**
 * \brief Draw a filled triangle between three three-dimensional points
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param p3 the third point
 * \param color the color of the triangle, in 5R6G5B color format
 */
void runderer_trianglef(runderer_t *run, vertex3f_t p1, vertex3f_t p2, vertex3f_t p3, unsigned int color) {
    vertex3f_t bc;
    vertex3f_t p;
    vertex3f_t normal;
    float intensity;

    // throw away the z coordinate
    vertex2i_t pi;
    vertex2i_t  pi1 = {p1[0], p1[1]},
                pi2 = {p2[0], p2[1]},
                pi3 = {p3[0], p3[1]};

    math_normal(p1, p2, p3, normal);
    math_normalize(normal);

    intensity = math_dotproduct(normal, run->light_direction);

    if (intensity < 0.0f) {
        printf("Culled back-face\n");
        return;
    }


    for (int y = 0; y < run->framebuffer->height; ++y) {
        for (int x = 0; x < run->framebuffer->width; ++x) {
            p[0] = x;
            p[1] = y;
            pi[0] = x;
            pi[1] = y;
            math_barycentric(pi1, pi2, pi3, pi, bc);
            if (bc[0] >= 0.0f && bc[1] >= 0.0f && bc[2] >= 0.0f) {
                p[2] = p1[2] * bc[0] + p2[2] * bc[1] + p3[2] * bc[2];
                if (run->zbuffer[y * run->framebuffer->width + x] < p[2]) {
                    run->zbuffer[y * run->framebuffer->width + x] = p[2];
                    run->framebuffer->buf16[y * run->framebuffer->width + x] = color;
                }
            }
        }
    }
}

/**
 * \brief Draw a filled triangle between three three-dimensional points
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param p3 the third point
 * \param color the color of the triangle, in 5R6G5B color format
 */
void runderer_triangle_flat(runderer_t *run, vertex3f_t p1, vertex3f_t p2, vertex3f_t p3, vertex3f_t color) {
    vertex3f_t bc;
    vertex3f_t p;
    vertex3f_t normal;
    float normalized;
    float intensity;
    unsigned int color_shaded;

    // throw away the z coordinate
    vertex2i_t pi;
    vertex2i_t  pi1 = {p1[0], p1[1]},
                pi2 = {p2[0], p2[1]},
                pi3 = {p3[0], p3[1]};

    math_normal(p1, p2, p3, normal);
    math_normalize(normal);

    intensity = math_dotproduct(normal, run->light_direction);

    if (intensity < 0.0f) {
        printf("Culled back-face\n");
        return;
    }

    color_shaded = rgb_to_565((unsigned int)(0xFF * intensity * color[0]),
                              (unsigned int)(0xFF * intensity * color[1]),
                              (unsigned int)(0xFF * intensity * color[2]));

    for (int y = 0; y < run->framebuffer->height; ++y) {
        for (int x = 0; x < run->framebuffer->width; ++x) {
            p[0] = x;
            p[1] = y;
            pi[0] = x;
            pi[1] = y;
            math_barycentric(pi1, pi2, pi3, pi, bc);
            if (bc[0] >= 0.0f && bc[1] >= 0.0f && bc[2] >= 0.0f) {
                p[2] = p1[2] * bc[0] + p2[2] * bc[1] + p3[2] * bc[2];
                if (run->zbuffer[y * run->framebuffer->width + x] < p[2]) {
                    run->zbuffer[y * run->framebuffer->width + x] = p[2];
                    run->framebuffer->buf16[y * run->framebuffer->width + x] = color_shaded;
                }
            }
        }
    }
}
