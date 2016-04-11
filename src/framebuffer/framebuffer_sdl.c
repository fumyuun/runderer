#include "framebuffer.h"
#include "math.h"
#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

static int framebuffers_active = 0;

typedef struct {
    SDL_Window *window;
    SDL_Surface *win_surface;
    SDL_Surface *win_buf;
}
sdl_fb_t;

int framebuffer_init(framebuffer_t *fb, unsigned int width, unsigned int height) {

    // Initialize SDL if we're the first framebuffer active
    if (framebuffers_active++ == 0 && SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Couldnt initialize SDL\n");
        return 1;
    }

    sdl_fb_t *sdl_fb = malloc(sizeof(sdl_fb));
    if (sdl_fb == NULL) {
        printf("Couldnt allocate opaque struct\n");
        SDL_Quit();
        return 2;
    }
    fb->opaque = (void*)sdl_fb;

    // allocate a window
    sdl_fb->window = SDL_CreateWindow("framebuffer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)width, (int)height, SDL_WINDOW_SHOWN);
    if (sdl_fb->window == NULL) {
        printf("Couldnt create a window\n");
        SDL_Quit();
        return 3;
    }

    // get a pointer to the windows' surface too
    sdl_fb->win_surface = SDL_GetWindowSurface(sdl_fb->window);
    if (sdl_fb->win_surface == NULL) {
        printf("Couldnt access the window surface?\n");
        SDL_DestroyWindow(sdl_fb->window);
        SDL_Quit();
        return 4;
    }

    // allocate a 16 bits buffer surface
    sdl_fb->win_buf = SDL_CreateRGBSurface(0, (int)width, (int)height, 16, 0, 0, 0, 0);
    if (sdl_fb->win_buf == NULL) {
        printf("Couldnt create an extra buffer\n");
        SDL_DestroyWindow(sdl_fb->window);
        SDL_Quit();
        return 5;
    }

    fb->buf = sdl_fb->win_buf->pixels;
    fb->buf16 = sdl_fb->win_buf->pixels;

    fb->width = (unsigned int)sdl_fb->win_buf->w;
    fb->height = (unsigned int)sdl_fb->win_buf->h;

    return 0;
}

void framebuffer_clear(framebuffer_t *fb) {
    sdl_fb_t *sdl_fb = (sdl_fb_t*)fb->opaque;
    SDL_FillRect(sdl_fb->win_buf, NULL, SDL_MapRGB(sdl_fb->win_buf->format, 0x00, 0x00, 0x00));
}


void framebuffer_quit(framebuffer_t *fb) {
    if (fb != NULL) {
        sdl_fb_t *sdl_fb = (sdl_fb_t*)fb->opaque;

        SDL_FreeSurface(sdl_fb->win_buf);
        SDL_DestroyWindow(sdl_fb->window);

        free(sdl_fb);
        fb->opaque = NULL;
        // If we're the last framebuffer device, free stuff
        if (framebuffers_active-- == 1) {
            SDL_Quit();
        }
    }
}

void framebuffer_flip(framebuffer_t *fb) {
    sdl_fb_t *sdl_fb = (sdl_fb_t*)fb->opaque;
    // blit our 16-bit buffer on the window and update it
    SDL_BlitSurface(sdl_fb->win_buf, NULL, sdl_fb->win_surface, NULL);
    SDL_UpdateWindowSurface(sdl_fb->window);
}
