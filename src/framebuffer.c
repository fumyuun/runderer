#include "framebuffer.h"
#include "math.h"
#include <stdio.h>

static int framebuffers_active = 0;

int framebuffer_init(framebuffer_t *fb, unsigned int width, unsigned int height) {
    // Initialize SDL if we're the first framebuffer active
    if (framebuffers_active++ == 0 && SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Couldnt initialize SDL\n");
        return 1;
    }

    // allocate a window
    fb->window = SDL_CreateWindow("framebuffer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)width, (int)height, SDL_WINDOW_SHOWN);
    if (fb->window == NULL) {
        printf("Couldnt create a window\n");
        SDL_Quit();
        return 2;
    }

    // get a pointer to the windows' surface too
    fb->win_surface = SDL_GetWindowSurface(fb->window);
    if (fb->win_surface == NULL) {
        printf("Couldnt access the window surface?\n");
        SDL_DestroyWindow(fb->window);
        SDL_Quit();
        return 3;
    }

    // allocate a 16 bits buffer surface
    fb->win_buf = SDL_CreateRGBSurface(0, (int)width, (int)height, 16, 0, 0, 0, 0);
    if (fb->win_buf == NULL) {
        printf("Couldnt create an extra buffer\n");
        SDL_DestroyWindow(fb->window);
        SDL_Quit();
        return 4;
    }

    fb->buf = fb->win_buf->pixels;
    fb->buf16 = fb->win_buf->pixels;

    fb->width = (unsigned int)fb->win_buf->w;
    fb->height = (unsigned int)fb->win_buf->h;

    return 0;
}

void framebuffer_clear(framebuffer_t *fb) {
    SDL_FillRect(fb->win_buf, NULL, SDL_MapRGB(fb->win_buf->format, 0x00, 0x00, 0x00));
}


void framebuffer_quit(framebuffer_t *fb) {
    if (fb != NULL) {
        SDL_FreeSurface(fb->win_buf);
        SDL_DestroyWindow(fb->window);
        // If we're the last framebuffer device, free stuff
        if (framebuffers_active-- == 1) {
            SDL_Quit();
        }
    }
}

void framebuffer_flip(framebuffer_t *fb) {
    // blit our 16-bit buffer on the window and update it
    SDL_BlitSurface(fb->win_buf, NULL, fb->win_surface, NULL);
    SDL_UpdateWindowSurface(fb->window);
}
