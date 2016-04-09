#ifndef GLAPI_H
#define GLAPI_H

/**
 * \file glapi.h
 * Defines GL-like api calls
 */

#include "runderer.h"

enum {
    GL_MODELVIEW,
    GL_PROJECTION
};

/**
 * Activate a runderer instance on which the api calls will operate.
 * Call this before using any call!
 * \param run the instance to activate
 */
void runderer_activate(runderer_t *run);

void glMatrixMode(int mode);

void glViewport(int x, int y, int w, int h);

void glRotatef(float angle, float x, float y, float z);

#endif /* GLAPI_H */
