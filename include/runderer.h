#ifndef RUNDERER_H
#define RUNDERER_H

/**
 * \file runderer.h
 * The main runderer instance. We have to bind it to a framebuffer instance.
 * Some 3D direct rendering functions are included for testing purposes: these
 * shapes are drawn directly on the screen without any perspective.
 */

#include "framebuffer.h"
#include "vertex.h"

#define ZBUF_TYPE float

typedef struct {
    framebuffer_t* framebuffer;
    ZBUF_TYPE *zbuffer;
    vertex3f_t light_direction;
} runderer_t;

/**
 * Bind the runderer instance to a framebuffer driver
 * \param run a pointer to the runderer instance to bind
 * \param fb a pointer to the framebuffer instance to bind
 * \return 0 if succesful, nonzero otherwise
 */
int runderer_bind(runderer_t *run, framebuffer_t *fb);

/**
 * \brief Draw a triangle between three three-dimensional points
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param p3 the third point
 * \param color the color of the triangle, in 5R6G5B color format
 */
void runderer_triangle(runderer_t *run, vertex3f_t p1, vertex3f_t p2, vertex3f_t p3, unsigned int color);

/**
 * \brief Draw a filled triangle between three three-dimensional points
 * with a constant color.
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param p3 the third point
 * \param color the color of the triangle, in 5R6G5B color format
 */
void runderer_trianglef(runderer_t *run, vertex3f_t p1, vertex3f_t p2, vertex3f_t p3, unsigned int color);

/**
 * \brief Draw a filled triangle between three three-dimensional points, using
 * a simple flat shaded color.
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param p3 the third point
 * \param color the color of the triangle, in 5R6G5B color format
 */
void runderer_triangle_flat(runderer_t *run, vertex3f_t p1, vertex3f_t p2, vertex3f_t p3, vertex3f_t color);


#endif
