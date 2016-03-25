#include "framebuffer.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int framebuffer_init(framebuffer_t *fb, unsigned int width, unsigned int height) {
    fb->display = XOpenDisplay(NULL);
    if (fb->display == NULL) {
        printf("Couldnt open display\n");
        return 1;
    }

    int blackColor = BlackPixel(fb->display, DefaultScreen(fb->display));
    int whiteColor = WhitePixel(fb->display, DefaultScreen(fb->display));

    fb->window = XCreateSimpleWindow(fb->display, DefaultRootWindow(fb->display), 0, 0,
        width, height, 0, blackColor, blackColor);

    XSelectInput(fb->display, fb->window, StructureNotifyMask);
    XMapWindow(fb->display, fb->window);

    fb->gc = XCreateGC(fb->display, fb->window, 0, NULL);
    if (fb->gc == NULL) {
        printf("Couldnt create a graphics context\n");
        return 2;
    }

    while (1) {
        XEvent e;
        XNextEvent(fb->display, &e);
        if (e.type == MapNotify)
            break;
    }

    fb->ximage = XCreateImage(fb->display, None, 24, ZPixmap, 0,  NULL, width, height, 8, 0);
    if (fb->ximage == NULL) {
        printf("Couldnt open ximage\n");
        return 3;
    }

    fb->buffer = malloc(height * fb->ximage->bytes_per_line);
    if (fb->buffer == NULL) {
        printf("Couldnt allocate a buffer\n");
        return 4;
    }

    fb->ximage->data = fb->buffer;
    fb->buf16 = fb->buffer;
    fb->buf32 = fb->buffer;

    XFlush(fb->display);
    return 0;
}

void framebuffer_quit(framebuffer_t *fb) {
    XDestroyImage(fb->ximage);
}

void framebuffer_flip(framebuffer_t *fb) {
    XPutImage(fb->display, fb->window, fb->gc, fb->ximage, 0, 0, 0, 0, fb->ximage->width, fb->ximage->height);
    XFlush(fb->display);
}

void framebuffer_rect(framebuffer_t *fb, int x, int y, int width, int height, int color) {
    for (int i = y; i < y + height; ++i) {
        for (int j = x; j < x + width; ++j) {
            fb->buf32[i * fb->ximage->width + j] = color;
        }
    }
}

