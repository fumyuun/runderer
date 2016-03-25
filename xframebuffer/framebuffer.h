#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <X11/Xlib.h>
#include <stdint.h>

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int pitch;
    Display *display;
    Window window;
    XImage *ximage;
    GC gc;
    void *buffer;
    uint32_t *buf32;
    uint16_t *buf16;
} framebuffer_t;

int framebuffer_init(framebuffer_t *fb, unsigned int width, unsigned int height);

void framebuffer_quit(framebuffer_t *fb);

void framebuffer_flip(framebuffer_t *fb);

void framebuffer_rect(framebuffer_t *fb, int x, int y, int width, int height, int color);

#endif /* FRAMEBUFFER_H */
