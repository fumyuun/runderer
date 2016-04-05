#include "gl.h"
#include "runderer.h"

// the static main instance of our runderer
static runderer_t *runderer = NULL;

void runderer_activate(runderer_t *run) {
    runderer = run;
}

void glViewport(int x, int y, int w, int h) {
    mat_identity(runderer->view);
    runderer->depth = 10.0f; // temporary value for now
    runderer->view[0] = w / 2.0f;
    runderer->view[4 * 0 + 3] = x + w / 2.0f;
    runderer->view[4 * 1 + 1] = h / 2.0f;
    runderer->view[4 * 1 + 3] = y + h / 2.0f;
    runderer->view[4 * 2 + 2] = runderer->depth / 2.0f;
    runderer->view[4 * 2 + 3] = runderer->depth / 2.0f;
}
