#include "framebuffer.h"
#include <stdio.h>

int framebuffer_init(framebuffer_t *fb, unsigned int width, unsigned int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Couldnt initialize SDL\n");
        return 1;
    }

    fb->window = SDL_CreateWindow("framebuffer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (fb->window == NULL) {
        printf("Couldnt create a window\n");
        SDL_Quit();
        return 2;
    }

    fb->win_surface = SDL_GetWindowSurface(fb->window);
    if (fb->win_surface == NULL) {
        printf("Couldnt access the window surface?\n");
        SDL_DestroyWindow(fb->window);
        SDL_Quit();
        return 3;
    }

    fb->win_buf = SDL_CreateRGBSurface(0, width, height, 16, 0, 0, 0, 0);
    if (fb->win_buf == NULL) {
        printf("Couldnt create an extra buffer\n");
        SDL_DestroyWindow(fb->window);
        SDL_Quit();
        return 4;
    }

    fb->buf = fb->win_buf->pixels;
    fb->buf16 = fb->win_buf->pixels;

    fb->width = fb->win_buf->w;
    fb->height = fb->win_buf->h;

    return 0;
}

void framebuffer_quit(framebuffer_t *fb) {
    if (fb != NULL) {
        SDL_FreeSurface(fb->win_buf);
        SDL_DestroyWindow(fb->window);
        SDL_Quit();
    }
}

void framebuffer_flip(framebuffer_t *fb) {
    SDL_BlitSurface(fb->win_buf, NULL, fb->win_surface, NULL);
    SDL_UpdateWindowSurface(fb->window);
}

void framebuffer_rect(framebuffer_t *fb, int x, int y, int width, int height, int color) {
    for (int i = y; i < y + height; ++i) {
        for (int j = x; j < x + width; ++j) {
            fb->buf16[i * fb->width + j] = color;
        }
    }
}