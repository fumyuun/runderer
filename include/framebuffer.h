#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/**
 * \file framebuffer.h
 * A simple framebuffer device emulation driver to test the renderer on.
 * Currently uses SDL as backend but abstracts away from this.
 *
 * To ensure that the renderer can always draw using 16-bits 5R6G5B colors,
 * independantly of our actual screen (window) pixel format, we use an
 * intermediate 16-bit buffer. Thus, this renderer works as if it's double
 * buffered, and we have to call the flip method to update to the screen.
 *
 * Also a few 2D direct rendering functions are offered that directly draw in
 * the framebuffer, for testing purposes.
 */

#include "SDL.h"
#include <stdint.h>
#include "vertex.h"

typedef struct {
    unsigned int width;
    unsigned int height;
    SDL_Window *window;
    SDL_Surface *win_surface;
    SDL_Surface *win_buf;
    void *buf;
    uint16_t *buf16;
} framebuffer_t;

/**
 * \brief Initialize the framebuffer emulation driver to a given screen size.
 * This method will also initialize SDL if needed.
 * \param fb a pointer to the framebuffer driver
 * \param width the width of the framebuffer
 * \param height the height of the framebuffer
 * \return 0 if succesful, nonzero otherwise
 */
int framebuffer_init(framebuffer_t *fb, unsigned int width, unsigned int height);

/**
 * \brief Clean up the framebuffer driver.
 * This method will also close SDL if there are no more active framebuffers.
 * \param fb a pointer to the framebuffer driver
 */
void framebuffer_quit(framebuffer_t *fb);

/**
 * \brief Update the framebuffer to the screen.
 * \param fb a pointer to the framebuffer driver
 */
void framebuffer_flip(framebuffer_t *fb);

/**
 * \brief Draw a rectangle directly on the framebuffer.
 * \param fb a pointer to the framebuffer driver
 * \param x the x position of the rectangle
 * \param y the y position of the rectangle
 * \param width the width of the rectangle
 * \param height the height of the rectangle
 * \param color the color of the rectangle, in 5R6G5B color format
 */
void framebuffer_rect(framebuffer_t *fb, int x, int y, int width, int height, int color);

/**
 * \brief Draw a line between two points
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param color the color of the line, in 5R6G5B color format
 */
void framebuffer_line(framebuffer_t *fb, vertex2i_t p1, vertex2i_t p2, unsigned int color);

/**
 * \brief Draw a triangle between three two-dimensional points
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param p3 the third point
 * \param color the color of the triangle, in 5R6G5B color format
 */
void framebuffer_triangle(framebuffer_t *fb, vertex2i_t p1, vertex2i_t p2, vertex2i_t p3, unsigned int color);

/**
 * \brief Draw a filled triangle between three two-dimensional points
 * \param fb a pointer to the framebuffer driver
 * \param p1 the first point
 * \param p2 the second point
 * \param p3 the third point
 * \param color the color of the triangle, in 5R6G5B color format
 */
void framebuffer_trianglef(framebuffer_t *fb, vertex2i_t p1, vertex2i_t p2, vertex2i_t p3, unsigned int color);

#endif /* FRAMEBUFFER_H */
