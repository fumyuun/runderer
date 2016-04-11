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

#include <stdint.h>
#include "vector.h"

typedef struct {
    unsigned int width;
    unsigned int height;
    void *opaque;
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
 * \brief Clear the framebuffer to black
 * \param fb a pointer to the framebuffer driver
 */
void framebuffer_clear(framebuffer_t *fb);

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

#endif /* FRAMEBUFFER_H */
