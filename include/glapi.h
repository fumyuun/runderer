#ifndef GLAPI_H
#define GLAPI_H

/**
 * \file glapi.h
 * Defines GL-like api calls
 */

#include "runderer.h"

enum {
	// matrix modes
	GL_MODELVIEW,
	GL_PROJECTION
};

enum {
	// enable modes
	GL_CULL_FACE      = 0x01,

	// culling modes
	GL_FRONT          = 0x02,
	GL_BACK           = 0x04,
	GL_FRONT_AND_BACK = 0x08
};

enum {
	// clear bits
	GL_COLOR_BUFFER_BIT = 0x01,
	GL_DEPTH_BUFFER_BIT = 0x02
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

void glEnable(int mode);
void glDisable(int mode);

void glCullFace(int mode);

void glClear(int mask);

#endif /* GLAPI_H */
