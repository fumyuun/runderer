#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "SDL.h"
#include <stdint.h>
#include "../vertex/vertex.h"

typedef struct {
    unsigned int width;
    unsigned int height;
    SDL_Window *window;
    SDL_Surface *win_surface;
    SDL_Surface *win_buf;
    void *buf;
    uint16_t *buf16;
} framebuffer_t;

int framebuffer_init(framebuffer_t *fb, unsigned int width, unsigned int height);

void framebuffer_quit(framebuffer_t *fb);

void framebuffer_flip(framebuffer_t *fb);

void framebuffer_rect(framebuffer_t *fb, int x, int y, int width, int height, int color);

void framebuffer_line(framebuffer_t *fb, vertex2i_t p1, vertex2i_t p2, unsigned int color);

void framebuffer_triangle(framebuffer_t *fb, vertex2i_t p1, vertex2i_t p2, vertex2i_t p3, unsigned int color);

#endif /* FRAMEBUFFER_H */
