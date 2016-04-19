#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "vector.h"
#include "runderer.h"

typedef struct {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
} bbox_t;

void get_bbox(vec3f_t p1, vec3f_t p2, vec3f_t p3, bbox_t *result);
void rasterize_triangle(struct runderer* self, stream_t p1, stream_t p2, stream_t p3, fragment_t* frag_buf_begin, fragment_t** frag_buf_end);
void rasterize_triangle_edge(struct runderer* self, stream_t p1, stream_t p2, stream_t p3, fragment_t* frag_buf_begin, fragment_t** frag_buf_end);

#endif /* RASTERIZER_H */
