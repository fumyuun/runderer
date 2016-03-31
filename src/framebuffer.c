#include "framebuffer.h"
#include "math.h"
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

void framebuffer_line(framebuffer_t *fb, vertex2i_t p1, vertex2i_t p2, unsigned int color) {
    // bresenham, based on wikipedia example
    #define line_gen(ptr) {\
        for (x = pmin[0]; x < pmax[0]; ++x) {\
            ptr[y*fb->width + x] = color;\
            error += deltaerr;\
            while (error > 0.5f) {\
                ptr[y*fb->width + x] = color;\
                y += ycorr;\
                error -= 1.0f;\
            }\
        }\
    }
    // sort points according to x position
    int *pmin = p1;
    int *pmax = p2;

    if (p1[0] > p2[0]) {
        pmin = p2;
        pmax = p1;

    }

    int deltax = abs(pmax[0] - pmin[0]);
    int deltay = abs(pmax[1] - pmin[1]);
    int ycorr = (pmax[1] - pmin[1]) > 0 ? 1 : -1;
    float error = 0.0;

    float deltaerr = ((float)deltay / (float)deltax);

    int x;
    int y = pmin[1];

    line_gen(fb->buf16);
}

void framebuffer_triangle(framebuffer_t *fb, vertex2i_t p1, vertex2i_t p2, vertex2i_t p3, unsigned int color) {
    framebuffer_line(fb, p1, p2, color);
    framebuffer_line(fb, p2, p3, color);
    framebuffer_line(fb, p3, p1, color);
}

void framebuffer_trianglef(framebuffer_t *fb, vertex2i_t p1, vertex2i_t p2, vertex2i_t p3, unsigned int color) {
    vertex3f_t bc;
    vertex2i_t p;
    for (int y = 0; y < fb->height; ++y) {
        for (int x = 0; x < fb->width; ++x) {
            p[0] = x;
            p[1] = y;
            math_barycentric(p1, p2, p3, p, bc);
            if (bc[0] >= 0.0f && bc[1] >= 0.0f && bc[2] >= 0.0f) {
                fb->buf16[y * fb->width + x] = color;
            }
        }
    }
}
