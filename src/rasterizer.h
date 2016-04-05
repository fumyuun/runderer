#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "vector.h"
#include "runderer.h"

void rasterize_triangle(struct runderer* self, stream_t p1, stream_t p2, stream_t p3, fragment_t* frag_buf_begin, fragment_t** frag_buf_end);

#endif /* RASTERIZER_H */
