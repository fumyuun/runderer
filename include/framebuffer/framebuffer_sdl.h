#ifndef FRAMEBUFFER_SDL_H
#define FRAMEBUFFER_SDL_H

/**
 * \file framebuffer_sdl.h
 * An SDL implementation for the framebuffer driver.
 */
#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct {
    unsigned int width;
    unsigned int height;
    SDL_Window *window;
    SDL_Surface *win_surface;
    SDL_Surface *win_buf;
    void *buf;
    uint16_t *buf16;
} framebuffer_t;

#endif /* FRAMEBUFFER_SDL_H */
